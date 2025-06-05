import socket
import threading
import time
import pytest

HOST = '127.0.0.1'
PORT = 8000
RESPONSE = b"SuperSecretSecret"

def tcp_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_sock:
        server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_sock.bind((HOST, PORT))
        server_sock.listen(1)
        conn, addr = server_sock.accept()
        with conn:
            _ = conn.recv(1024)  # Receive the incoming packet
            conn.sendall(RESPONSE)

@pytest.fixture(scope="module")
def start_server():
    thread = threading.Thread(target=tcp_server, daemon=True)
    thread.start()
    time.sleep(0.1)  # Give server time to start
    yield
    # Server thread exits after one connection

def test_tcp_response(start_server):
    with socket.create_connection((HOST, PORT), timeout=2) as client_sock:
        client_sock.sendall(b"HelloServer")
        data = client_sock.recv(1024)
        assert data == RESPONSE
