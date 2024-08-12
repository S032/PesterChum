FROM ubuntu:24.10

RUN apt-get update && \
    apt-get install -y \
    cmake \
    build-essential \
    libmysqlclient-dev \
    git \
    libmysqlclient-dev && \
    apt-get clean

RUN git clone https://github.com/S032/PesterChum.git

CMD ["PesterChum/UNIX_Server/./server"]
