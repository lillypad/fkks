#!/usr/bin/env python3

import os
import time
from flask import Flask
from flask import Response
from flask import request

app = Flask(__name__)

@app.route('/BOH9KGa4jvUsU4jL/gate.php', methods=['GET'])
def checkin():
	resp = Response("enhFA1kHQHsAaWUDW3xQARQWMHckfzwHbgdlBVhjWAhlf0McWmkuDnQUHX8nEj5ROzkQUxxXLhV2ChVwKAgzbxRjWl4HUV1gHywsRkYSPncZCDZwLWQuFRQ5JEIOLBVRbhYrdTp3Mwh0CgttWhI+dxkINnAtZC4VAGhmBjUSJXUHADl/N2kVL0IzIF0aEhVIPxYrdTp3Mwh0CgttQGMVSD9lK20vZDAIcx0GbTVoFEMuOwRAB1AYJlR9CHYPPgpEJDkrbS9kMAhzHQZtNX0+bwwbNXAqcyMVbmlhAjUSJXUHADl/N2kuFXUdGHsnAD5vFBYwdyR/PAduBw==")
	resp.headers['Server'] = 'nginx/1.14.1'
	resp.headers['Content-Type'] = 'text/html; charset=UTF-8'
	#resp.headers['Transfer-Encoding'] = 'chunked'
	#resp.headers['Connection'] = 'close'
	resp.headers['X-Powered-By'] = 'PHP/7.1.29'
	return resp

@app.route('/BOH9KGa4jvUsU4jL/gate.php', methods=['POST'])
def exfiltrate():
	print(request.headers)
	date = time.strftime("%Y-%m-%dT%H:%M:%S")
	fp = open(date + '.bin', 'wb')
	fp.write(request.data)
	fp.close()
	os.system('hexdump -C ' + date + '.bin')
	resp = Response('OK')
	resp.headers['Server'] = 'nginx/1.14.1'
	resp.headers['X-Powered-By'] = 'PHP/7.1.1.29'
	return resp

if __name__ == '__main__':
	app.run(host='0.0.0.0', debug=True, port=80)
