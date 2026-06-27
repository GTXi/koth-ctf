FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    gcc \
    libmariadb-dev \
    libmariadb-dev-compat \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY server1.c server2.c ./

RUN gcc -o server1 server1.c $(mysql_config --cflags --libs) -Wall && \
    gcc -o server2 server2.c $(mysql_config --cflags --libs) -Wall

CMD ["./server1"]
