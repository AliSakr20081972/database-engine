FROM python:3.11-slim
WORKDIR /app
COPY app.py .
RUN pip install --no-cache-dir flask
ENV DB_HOST=database
ENV DB_PORT=5432
EXPOSE 5000
CMD ["python", "app.py"]
