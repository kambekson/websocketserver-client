import socket
import time

HOST = '10.83.10.107'  # Change this to the IP address of your server
PORT = 20248

def send_message(sock, message):
    sock.sendall(message.encode('utf-8'))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    initial_message = "Your IP is 10.83.10.107\n" # Change this to the IP address of your server
    send_message(s, initial_message)

    while True:
        user_input = input("Введите сообщение: ")
        send_message(s, user_input + "\n")
