import os

def recieve():
    while True:
        commands = None
        if not os.path.isfile('command.lock') and os.path.isfile('command.txt'):
            with open('command.lock', 'w') as lock_file:
                with open('command.txt', 'r') as cmd_file:
                    commands = cmd_file.readlines()
            try:
                os.remove('command.txt')
            except FileNotFoundError:
                pass
            try:
                os.remove('command.lock')
            except FileNotFoundError:
                pass
        if commands:
            for command in commands:
                yield command[:-1] # [-1] needed to remove \n
        yield

def input_event_loop():
    commands = []
    while True:
        cmd = input()
        commands.append(cmd) if cmd is not None and cmd != '' else False
        if not os.path.isfile('command.lock') and commands != []:
            with open('command.lock', 'w') as lock_file:
                with open('command.txt', 'w') as cmd_file:
                    for command in commands:
                        cmd_file.write(command + '\n')
                    commands = []
            os.remove('command.lock')
