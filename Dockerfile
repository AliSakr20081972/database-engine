# Build stage
FROM gcc:12 as build
WORKDIR /usr/src/app
COPY . .
RUN make

# Runtime stage
FROM debian:stable-slim
WORKDIR /app
COPY --from=build /usr/src/app/db .
RUN apt-get update && apt-get install -y socat && rm -rf /var/lib/apt/lists/*
EXPOSE 5432
CMD ["socat", "tcp-l:5432,reuseaddr,fork", "EXEC:./db --cli-only,pty,stderr,echo=0"]
