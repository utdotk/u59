className = raw_input()
methods = []
while True:
    row = raw_input()
    if row == '':
        break
    methods.append(row.split())

for method in methods:
    outStr = ''
    i = 1 if method[0] == 'virtual' else 0
    outStr += method[i] + ' ' + className + '::'
    while i < len(method) - 1:
        i += 1
        outStr += method[i] + ' ' if i < len(method) - 1 else method[i].rstrip(';')
    
    outStr += ' {\n}\n'
    print(outStr)
