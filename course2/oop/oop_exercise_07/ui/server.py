from flask import Flask
from flask import send_from_directory
from flask import render_template
import json

app = Flask(__name__)

@app.route('/')
def index():
	return render_template('./index.html')

@app.route('/test.json')
def get_file():
	s = ''
	f = open('test.json', 'r')
	s = json.load(f)
	f.close()
	return json.dumps(s)

if __name__ == "__main__":
    app.run()