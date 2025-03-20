require 'sinatra'
require 'json'

set :port, 4567


post '/sensor_data' do
	data = JSON.parse(request.body.read)
	puts "Recived sensor data #{data}"
end
get '/' do
	@temperature = 23.5
	erb :index
end

__END__

@@ index
<html>
	<body>
		<h1>IoT Data</h1>
		<p>Aktuell temperatur: <%= @temperarute %>C</p>
	</body>
</html>

	


