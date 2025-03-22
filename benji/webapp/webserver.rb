require 'sinatra'
require 'json'

enable :sessions

set :port, 4567
set :views, File.dirname(__FILE__) + '/views'
set :public_folder, File.dirname(__FILE__) + '/public'

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



get '/' do
	erb :login
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

get '/sub_pages/:page' do
	if session[:user]
		erb :"sub_pages/#{params[:page]}"
	else
		redirect '/'
	end
end
