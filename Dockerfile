FROM gcc:12 AS initial

WORKDIR /usr/src/myapp

COPY . .

RUN make -B

FROM debian:bookworm-slim

WORKDIR /usr/src/myapp

COPY --from=initial /usr/src/myapp/lab7 .

ENTRYPOINT [ "./lab7" ]