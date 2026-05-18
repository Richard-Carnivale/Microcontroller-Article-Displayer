import requests
import serial
import time

API_KEY = "ac044938cac547559428e143b16ef0fa"
URL = "https://newsapi.org/v2/top-headlines"

ser_tiva = serial.Serial('/dev/ttyS4', baudrate=115200, timeout=1)

while True:
    params = {
        "apiKey": API_KEY,
        "country": "us",
        "category": None,
        "q": None
    }

    while params["category"] is None:
        reply = ser_tiva.readline().decode(errors="ignore").strip()
        if reply:
            if reply.lower() == "reset":
                break
            params["category"] = reply
    if params["category"] is None:
        continue

    #print("success:", params["category"])

    while True:
        reply = ser_tiva.readline().decode(errors="ignore").strip()
        #print(reply)
        if reply:
            if reply.lower() == "reset":
                break
            elif reply == "n" or reply == "":
                break
            else:
                params["q"] = reply
                break
    if reply.lower() == "reset":
        continue

    response = requests.get(URL, params=params)

    if response.status_code == 200:
        data = response.json()
        articles = data.get("articles", [])
        total_results = data.get("totalResults", 0)

        if articles:
            #print(f"Found {total_results} articles for category '{params['category']}' and query '{params['q']}'")
            for idx, article in enumerate(articles[:5], start=1):
                headline = article.get("title", "No title")
                description = article.get("description", "No description")
                combined = f"Headline {idx}: {headline}\nDescription: {description}"
                #print(combined)
                ser_tiva.write((combined + "\n").encode("utf-8"))
        else:
            msg = f"No articles found for category '{params['category']}' and query '{params['q']}'"
            #print(msg)
            ser_tiva.write((msg + "\n").encode("utf-8"))
    else:
        msg = f"Request failed with status: {response.status_code}"
        #print(msg)
        ser_tiva.write((msg + "\n").encode("utf-8"))

    time.sleep(1)

ser_tiva.close()
