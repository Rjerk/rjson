# A simple json parser for practice.

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

### array (undo)
- [value ... ]
