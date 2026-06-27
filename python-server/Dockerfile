FROM python:3.11-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY server1.py server2.py server3.py ./

CMD ["python", "server1.py"]
