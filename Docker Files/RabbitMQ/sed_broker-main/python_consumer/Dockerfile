FROM python:3.9-slim

WORKDIR /python_consumer_module

COPY . .

RUN pip install pika

CMD ["python", "consumer.py"]