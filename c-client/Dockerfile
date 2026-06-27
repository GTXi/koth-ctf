FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    gcc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY client1.c client2.c ./

RUN gcc -o client1 client1.c -Wall && \
    gcc -o client2 client2.c -Wall

CMD ["./client1"]
