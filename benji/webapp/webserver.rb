require 'sinatra'
require 'json'
require 'mqtt'
require 'thread'

enable :sessions

set :port, 4567
set :views, File.dirname(__FILE__) + '/views'
set :public_folder, File.dirname(__FILE__) + '/public'

$latest_mqtt_data = {}
$mqtt_mutex = Mutex.new


configure do
  set :view_folders, [settings.views, File.join(settings.views, "sub_pages")]
end

# Anpassad metod för att söka i flera mappar
def find_template(views, filename, locals)
  settings.view_folders.each do |view_folder|
    path = File.join(view_folder, filename)
    return path if File.exist?(path)
  end
  nil # Om ingen fil hittas
end

Thread.new do
	MQTT::Client.connect('94.255.229.51', ack_timeout: 30) do |client|
		client.subscribe('sensors/simon-pico') #### topic here

		client.get do |topic, message|
			message = message.force_encoding('UTF-8')
				.encode('UTF-8', invalid: :replace, undef: :replace, replace: '?')
			puts "Recieved from #{topic}: #{message}"
		if message.strip.start_with?('{')
			begin
				require 'json'
				data = JSON.parse(message)
				$mqtt_mutex.synchronize do
  					$latest_mqtt_data[topic] = data
end
			puts "Stored data for #{topic}: #{$latest_mqtt_data[topic]}"
			rescue JSON::ParserError => e
				puts "Could not parse JSON: #{e.message}"
			end	
			end
		end
	end
end

get '/' do
	erb :login
end

get '/mqtt_data' do
  content_type :json
  $mqtt_mutex.synchronize do
    $latest_mqtt_data.to_json
  end
end

post '/check_password' do
	content_type :json
	data = JSON.parse(request.body.read)
	password = data["password"]
	result = `python3 password_checker.py #{password}`
	result
end

post '/login' do
	username = params[:username]
	password = params[:pwd]
	
	if username == "admin" && password == "embedded"
		session[:user] = username
		redirect '/main_page'
	else
		redirect '/'
	end
end

get '/main_page' do
	if session[:user]
		@username = session[:user]
		erb :main_page
	else
		redirect '/'
	end
end	

get '/create_account' do
	erb :create_account
end

get '/sub_pages/:page' do
	if session[:user]
		erb :"sub_pages/#{params[:page]}"
	else
		redirect '/'
	end
end
