import os
import socket
from flask import Flask, request, render_template_string

DB_HOST = os.environ.get("DB_HOST", "database")
DB_PORT = int(os.environ.get("DB_PORT", 5432))

app = Flask(__name__)

HTML = """
<!doctype html>
<title>Database UI</title>
<h1>Database UI</h1>
<form method="post">
  <input name="query" style="width:400px">
  <input type="submit" value="Run">
</form>
<pre>{{result}}</pre>
"""

def send_query(query):
    with socket.create_connection((DB_HOST, DB_PORT)) as sock:
        sock.sendall((query + "\nquit\n").encode())
        sock.shutdown(socket.SHUT_WR)
        data = b""
        while True:
            chunk = sock.recv(4096)
            if not chunk:
                break
            data += chunk
    text = data.decode(errors="ignore")
    lines = [line for line in text.splitlines() if line and not line.startswith("db>") and "Enter SQL" not in line]
    return "\n".join(lines)

@app.route('/', methods=['GET', 'POST'])
def index():
    result = ''
    if request.method == 'POST':
        q = request.form.get('query', '')
        if q:
            result = send_query(q)
    return render_template_string(HTML, result=result)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
