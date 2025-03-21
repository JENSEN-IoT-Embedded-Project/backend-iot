require 'sinatra'
require 'json'

enable :sessions

set :port, 4567
set :views, File.dirname(__FILE__) + '/views'
set :public_folder, File.dirname(__FILE__) + '/public'

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


