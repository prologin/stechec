/**
 * viewer.js - Javascript script for the viewer module
 * By Zopieux
 */

var PLAYING = false, TURN_ID = 1, CACHE = {}, TID, STIME = 300, PBS = false;

function setTime(time) {
    $('#i_time').text(time + '/' + NB_TURNS);
}

function turnTimer(time, func) {
    if(time == false) {
        clearTimeout(TID);
    } else
        TID = setTimeout(func, time);
}

function getTurn(turn_id, onready) {
    $.getJSON('/viewer/json/' + MATCH_ID + '/' + turn_id, onready);
}
function startCache(turn) {
    if(turn in CACHE && turn < NB_TURNS)
        startCache(turn+1);
    else {
        getTurn(turn, function(data) {
            CACHE[turn] = data;
            $('#i_cache').text(Math.round((Object.size(CACHE)/NB_TURNS*100)) + '%');
            if(TURN_ID == turn)
                drawMap(data);
            //startCache(turn+1);
        });
    }
}

function setPixel(x, y, kind, text) {
    PIXELS[y][x] = kind;
    var id = $('#p_' + x + '_' + y);
    if(kind != null) id.css('background-position', COORDS[kind]);
    if(text != null) id.text(text);
}

function drawMap(data) {
    $('#map div.cell').text('');
    var rows = data.m.split('|');
    for(var y = 0; y < rows.length; y ++) {
        for(var x = 0; x < rows[y].length; x ++) {
            var cha = GRAMMAR[rows[y].substring(x, x+1) || ' '];
            setPixel(x, y, cha, null);
        }
    }
    for(var bomb in data.b) {
        var b = data.b[bomb];
        setPixel(b[0], b[1], 'bomb', b[2]);
    }
    for(var meche in data.e) {
        var m = data.e[meche];
        setPixel(m[0], m[1], null, m[2]);
    }
}

function loadMap() {
    if(TURN_ID < 1 || TURN_ID > NB_TURNS) return false;
    if(TURN_ID in CACHE)
        drawMap(CACHE[TURN_ID]);
    else {
        startCache(TURN_ID);
    }
}

function playPauseMatch(pp) {
    if(pp != null) PLAYING = !pp;
    if(PLAYING) {
        turnTimer(false);
        PLAYING = false;
        $('#play').removeClass('pause');
        $('div.ui-slider-range').removeClass('slider-working');
    } else {
        if(TURN_ID == NB_TURNS)
            return false;
        PLAYING = true;
        $('#play').addClass('pause');
        $('div.ui-slider-range').addClass('slider-working');
        turnTimer(1, function() { toTurn(0, false); });
    }
}

function toTurn(index, absolute) {
    if((absolute && (index < 1 || index > NB_TURNS)) || (!absolute && (TURN_ID+index < 1 || TURN_ID+index > NB_TURNS)))
        return false;
    TURN_ID = (absolute) ? index : TURN_ID + index;
    setTime(TURN_ID);
    $('#time_slider').slider('value', TURN_ID);
    loadMap();
    if(PLAYING) {
        if((absolute && index > NB_TURNS) || (!absolute && TURN_ID+index > NB_TURNS)) {
            playPauseMatch(false); // stop
            return false;
        } else
            turnTimer(STIME, function() { toTurn(1, false); });
    }
}

$(document).ready(function() {

PIXELS = [];

$.extend($.ui.slider.defaults, {range: 'min'});

$('#play').click(function() { playPauseMatch(null) });
$('#first').click(function() { playPauseMatch(false); toTurn(1, true); });
$('#next').click(function() { playPauseMatch(false); toTurn(1, false); });
$('#previous').click(function() { playPauseMatch(false); toTurn(-1, false); });
$('#last').click(function() { playPauseMatch(false); toTurn(NB_TURNS, true); });
$('#time_slider').slider({value: 1, min: 1, max: NB_TURNS, step: 1, animate: true, stop: function(ev, ui) {
    toTurn(ui.value, true);
    if(PBS) {
        PBD = false;
        playPauseMatch(true);
    }
}, slide: function(ev, ui) {
    setTime(ui.value);
    if(PLAYING) PBS = true;
    playPauseMatch(false);
}});

for(var y = 0; y < CONF.sizey; y ++) {
    var row = $('<div class="row"></div>');
    var pixrow = [];
    for(var x = 0; x < CONF.sizex; x ++) {
        var cell = $('<div class="cell"></div>');
        cell.attr('id', 'p_' + x + '_' + y);
        cell.css('background-position', COORDS['empty']);
        row.append(cell);
        pixrow.push('empty');
    }
    PIXELS.push(pixrow);
    $('#map').append(row);
}

startCache(1);
//toTurn(1, true);

});
