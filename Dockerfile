FROM alpine:3.20 AS builder
RUN apk add --no-cache build-base
WORKDIR /app
COPY . .
RUN make && strip build/bin/*

FROM alpine:3.20
COPY --from=builder /app/build/bin /usr/local/bin/
COPY --from=builder /app/tmp /ficheiros/
WORKDIR /ficheiros
ENTRYPOINT ["interpretador"]
