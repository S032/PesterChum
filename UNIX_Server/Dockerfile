FROM ubuntu:24.10

EXPOSE 34543

RUN apt-get update && \
    apt-get install -y \
    cmake \
    build-essential \
    libmysqlclient-dev \
    libmysqlcppconn-dev && \
    apt-get clean

COPY . /PesterChum
WORKDIR /PesterChum

RUN cmake . && \
    cmake --build .

CMD ["./PesterServer"]
