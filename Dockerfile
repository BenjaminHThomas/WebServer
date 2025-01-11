FROM gcc:13.2

COPY . /webserv

WORKDIR /webserv

RUN make all

# Here you can expose the ports you want to use, based on your config file
EXPOSE 8080

# You can run the server with the config file you want to use
CMD ["./webserv", "config/config.json"]
