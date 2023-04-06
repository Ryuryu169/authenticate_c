FROM gcc:latest

COPY . /app
WORKDIR /app

RUN gcc -o app src/main.c

CMD ["./app"]