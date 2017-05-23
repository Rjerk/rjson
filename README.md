# A simple json parser for practice.

## Compile && Run

Build project.
```
make
```

Read json from file.
```
./rjson test.json
```

## Parser Support

### literal
- null
- false
- true

### string
- any unicode character
- escape character \" \/ \t , etc.
- \u 4-hex digits

### number
- regex: -?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?

### object
- {string : value}

### array
- [value ... ]

### json generator
- generator json text

## Reference
> https://github.com/miloyip/json-tutorial
