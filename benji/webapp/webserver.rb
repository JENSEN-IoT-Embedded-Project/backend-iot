require 'sinatra'
require 'json'

set :port, 4567
set :views, File.dirname(__FILE__) + '/views'
set :public_folder, File.dirname(__FILE__) + '/public'


post '/sensor_data' do
	data = JSON.parse(request.body.read)
	puts "Recived sensor data #{data}"
end
get '/' do
	@temperature = 23.5
	erb :index
end


	


