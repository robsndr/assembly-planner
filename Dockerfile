FROM gcc:9
COPY . /aoplanner/
WORKDIR /aoplanner
RUN set -ex;                    \
    apt-get update;             \
    apt-get install -y cmake                                                      
RUN cmake .; make
ENTRYPOINT [ "./planner" ]