/**
 * Common JS vars for map module
 **/

var CONF = {sizex: 25, sizey: 25, sprite_size: 16};
var GRAMMAR = {' ': 'empty', '#': 'wall', '$': 'soft', '+': 'mesh', 'A': 'p1', 'B': 'p2', 'C': 'p3', 'D': 'p4'};
var GRAMMAR2 = {'wall': '#', 'soft': '$', 'mesh': '+', 'empty': ' ', 'p1': 'A', 'p2': 'B', 'p3': 'C', 'p4': 'D'};
var TYPE2CHAR = ['empty', 'wall', 'soft', 'mesh', 'p1', 'p2', 'p3', 'p4'];
var COORDS = {
	empty: '0 0',
	wall: '-16px 0',
 	soft: '-32px 0',
	mesh: '-48px 0',
	p1: '-64px 0',
	p2: '-80px 0',
	p3: '-96px 0',
	p4: '-112px 0',
	bomb: '-128px 0',
}

Object.size = function(obj) {
    var size = 0, key;
    for(key in obj)
        if(obj.hasOwnProperty(key)) size++;
    return size;
};

