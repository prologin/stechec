/**
 * Common JS vars for map module
 **/

var CONF = {sizex: 19, sizey: 19, sprite_size: 32};

var COORDS = {
    'empty': '0 0',
    'chat': '-32px 0',
    'kangourou': '-64px 0',
    'singe': '-96px 0',
    'perroquet': '-128px 0',
    'nonexistant': '-160px 0',
};

Object.size = function(obj) {
    var size = 0, key;
    for(key in obj)
        if(obj.hasOwnProperty(key)) size++;
    return size;
};

