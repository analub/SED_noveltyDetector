import pika
import json
import os
import time
from collections import deque

RABBITMQ_HOST = "rabbitmq"
EXCHANGE_NAME = os.getenv('R_KEY1')
EXCHANGE_TYPE = "topic"
QUEUE_NAME = "final_consumer"
ROUTING_KEY = os.getenv('R_KEY1')
MAX_QUEUE_LENGTH = 10
MAX_BUFFER_LENGTH = 16

queue = deque(maxlen=MAX_QUEUE_LENGTH)

def callback(ch, method, properties, body):

    global queue

    json_message = json.loads(body.decode())
    
    queue.append(json_message)

    # Place your code here:
    
    # Example:
    # for item in queue:
        
    #    print(item)
     
    ch.basic_ack(delivery_tag=method.delivery_tag)

while True:

    credentials = pika.PlainCredentials("dam", "password")
    parameters = pika.ConnectionParameters(host='rabbitmq',
                                    port=5672,
                                    credentials=credentials)

    while True:
        try:
            
            connection = pika.BlockingConnection(parameters)
            channel = connection.channel()

            channel.queue_declare(queue=QUEUE_NAME, durable=True)

            channel.queue_bind(exchange=EXCHANGE_NAME, queue=QUEUE_NAME, routing_key=ROUTING_KEY)
            print(f"Queue '{QUEUE_NAME}' is now bound to exchange '{EXCHANGE_NAME}' with routing key '{ROUTING_KEY}'.")
            print(f"Waiting for messages in queue '{QUEUE_NAME}'. To exit, press CTRL+C.")
            channel.basic_consume(queue=QUEUE_NAME, on_message_callback=callback)

            channel.start_consuming()
        
        except Exception as e:

            print("Unable to connect...")
            print(e)
            time.sleep(5)