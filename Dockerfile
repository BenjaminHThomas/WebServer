FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    libcurl4-openssl-dev \
    openssl \
    libssl-dev \
    bash \
    procps \
    net-tools \
    python3 \
    php-cgi \
    php-cli \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /webserv

COPY . /webserv

RUN chown -R root:root /webserv

WORKDIR /webserv

RUN make all

# Here you can expose the ports you want to use, based on your config file
EXPOSE 8080

# You can run the server with the config file you want to use
CMD ["./webserv", "config/config.json"]
