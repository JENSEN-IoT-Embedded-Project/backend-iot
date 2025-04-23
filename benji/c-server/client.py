import socket

# Skapa en TCP-socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Anslut till servern (här antas servern köras på localhost och port 8080)
client_socket.connect(('localhost', 8080))

# Skapa HTTP GET-förfrågan
http_request = """DELETE /index.html HTTP/1.1\r\n\
Host: localhost:8080\r\n\
User-Agent: PythonSocketClient\r\n\
Accept: text/html\r\n\
Connection: close\r\n\
\r\n"""

# Skicka HTTP GET-förfrågan till servern
client_socket.sendall(http_request.encode())

# Ta emot och skriv ut serverns svar
response = client_socket.recv(4096)
print("Server Response:")
print(response.decode())

# Stäng anslutningen
client_socket.close()
