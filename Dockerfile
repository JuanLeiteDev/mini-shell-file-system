FROM gcc:14 AS builder
WORKDIR /app
COPY . .
RUN make

FROM alpine:3.20
COPY --from=builder /app/build/bin /usr/local/bin/
COPY --from=builder /app/tmp /ficheiros/
WORKDIR /ficheiros
ENTRYPOINT ["interpretador"]
