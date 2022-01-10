with open('long_locals.lox', 'w') as f:
    f.write('{\n')
    for i in range(257):
        f.write(f'    var local{i};\n')
    f.write('}\n')
