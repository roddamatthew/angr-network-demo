import socket
import threading
import time
import subprocess
import pytest

HOST = '127.0.0.1'
PORT = 8000

def tcp_server(response_bytes):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_sock:
        server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_sock.bind((HOST, PORT))
        server_sock.listen(1)
        conn, _ = server_sock.accept()
        with conn:
            _ = conn.recv(1024)
            conn.sendall(response_bytes)

@pytest.fixture
def start_server(request):
    response = request.param.encode()
    thread = threading.Thread(target=tcp_server, args=(response,), daemon=True)
    thread.start()
    time.sleep(0.1)  # Give server time to start
    yield
    # Thread exits after one connection

@pytest.mark.parametrize("start_server,response_text", [
    ("SuperSecretSecret\n", "SuperSecretSecret\n"),
    ("TopSecretValue", "TopSecretValue"),
], indirect=["start_server"])
def test_tcp_response(start_server, response_text):
    # Run the executable and capture stdout
    result = subprocess.run(
        ["./build/main.exe"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,  # decode output to string
        timeout=5
    )

    # Check the binary produced the expected response
    if response_text == "SuperSecretSecret\n":
        assert "Success" in result.stdout
    else:
        assert "Failure" in result.stdout