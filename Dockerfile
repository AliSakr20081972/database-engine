# Build stage
FROM gcc:12 as build
WORKDIR /usr/src/app
COPY . .
RUN make

# Runtime stage
FROM debian:stable-slim
WORKDIR /app
COPY --from=build /usr/src/app/db .
CMD ["./db"]
