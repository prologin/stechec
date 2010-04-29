/**
 * viewer2.js - Javascript script for the viewer module, improved
 * By Zopieux
 */

// STIME is delay between two played frames; DTIME is delay between two downloaded frames
var PLAYING = false, OFRAME = 0, FRAME = 0, CACHE_D = [], TEXTUED = [], EMPTYME = [], TID, DID, STIME = 400, DTIME = 10, PBS = false, DLING = 2, DLED;

function setTime(time) {
    $('#i_time').text(time + '/' + NB_FRAMES);
}

function turnTimer(time, func) {
    if(time == false) {
        clearTimeout(TID);
    } else
        TID = setTimeout(func, time);
}

function download(before, after, done) {
    $.getJSON('/viewer/json/' + MATCH_ID + '/' + before + '/' + after, done);
}

function cacheFirstNull() {
    for(var f=1; f < NB_FRAMES; f++)
        if(getCache(f-1, f) == null) return f;
    return f;
}
function countCache() {
    var count = 0;
    for(var y=0; y < CACHE_D.length; y++) {
        if(CACHE_D[y] == null) continue;
        for(var x=0; x < CACHE_D[y].length; x++) {
            if(CACHE_D[y][x] != null) count++;
        }
    }
    return count;
}

function dlFrame(before, after) {
    clearTimeout(DID);
    if(before && after) {
        download(before, after, function(data) {
            var sli = this.url.split('/');
            setCache(sli[4], sli[5], data);
            drawMap(data);
            DID = setTimeout(dlFrame, DTIME);
        });
    } else {
        // cache
        if(DLING > 1 && (DLING == NB_FRAMES+1 || getCache(DLING-1, DLING) != null)) {
            var firstNull = cacheFirstNull();
            if(firstNull == NB_FRAMES) { clearTimeout(DID); return; }
            DLING = firstNull;
        }
        download(DLING-1, DLING, function(data) {
            var sli = this.url.split('/');
            setCache(sli[4], sli[5], data);
            DLING ++;
            $('#i_cache').text(Math.round(countCache()/NB_FRAMES*100) + '%');
            DID = setTimeout(dlFrame, DTIME); // pourquoi attendre ? :D
        });
    }
}

function setCache(before, after, data) {
    if(CACHE_D[before] == null) CACHE_D[before] = [];
    if(CACHE_D[before][after] == null)
        CACHE_D[before][after] = data;
}
function getCache(before, after) {
    if(before == 0 && after == 1) // first frame from HTML
        return INITIAL_FRAME;
    if(CACHE_D[before] == null || CACHE_D[before][after] == null)
        return null;
    return CACHE_D[before][after];
}

function updateMap(before, after) {
    if(FRAME < 1 || FRAME > NB_FRAMES) return;
    var cached = getCache(before, after);
    if(cached == null)
        dlFrame(before, after);
    else
        drawMap(cached);
}

function drawMap(diff) {
    if(diff.m != null) { // from HTML
        var cells = diff.m, bombs = diff.b, meches = diff.e;
        for(var y = 0; y < CONF.sizey; y++) {
            for(var x = 0; x < CONF.sizex; x++) {
                PIXELS[y][x].css('background-position', COORDS[GRAMMAR[cells.substr((y*25+x), 1)]]);
            }
        }
    } else {
        var diffs = diff[0], bombs = diff[1], meches = diff[2];
        //var clone = $('#map').clone();
        for(d in diffs) {
            var h = diffs[d];
            PIXELS[h[1]][h[0]].css('background-position', COORDS[TYPE2CHAR[h[2]]]);
        }
    }
    for(t in TEXTUED)
        TEXTUED[t].text('');
    TEXTUED = [];
    for(e in EMPTYME)
        EMPTYME[e].css('background-position', COORDS['empty']);
    EMPTYME = [];
    for(b in bombs) {
        var h = bombs[b];
        PIXELS[h[1]][h[0]].css('background-position', COORDS['bomb']).text(h[2]);
        TEXTUED.push(PIXELS[h[1]][h[0]]);
        EMPTYME.push(PIXELS[h[1]][h[0]]);
    }
    for(m in meches) {
        var h = meches[m];
        PIXELS[h[1]][h[0]].text(h[2]);
        TEXTUED.push(PIXELS[h[1]][h[0]]);
    }
    if(PLAYING)
        turnTimer(STIME, function() { toFrame(1, false); });
}

function playPause(pp) {
    if(pp != null) PLAYING = !pp;
    if(PLAYING) {
        turnTimer(false);
        PLAYING = false;
        $('#play').removeClass('pause');
        $('div.ui-slider-range').removeClass('slider-working');
    } else {
        if(FRAME == NB_FRAMES)
            return;
        PLAYING = true;
        $('#play').addClass('pause');
        $('div.ui-slider-range').addClass('slider-working');
        turnTimer(1, function() { toFrame(0, false); });
    }
}

function toFrame(index, absolute) {
    if((absolute && (index < 1 || index > NB_FRAMES || index == FRAME)) || (!absolute && (FRAME+index < 1 || FRAME+index > NB_FRAMES)))
        return;
    OFRAME = FRAME;
    FRAME = (absolute) ? index : FRAME + index;
    setTime(FRAME);
    updateMap(OFRAME, FRAME);
    $('#time_slider').slider('value', FRAME);
    if(PLAYING) {
        if((absolute && index > NB_FRAMES) || (!absolute && FRAME+index > NB_FRAMES)) {
            $('#viewer_wrap #msg').fadeIn();
            playPause(false); // stop
            return;
        }
    }
    if(absolute && index > 1) DLING = index;
}

$(document).ready(function() {

$.extend($.ui.slider.defaults, {range: 'min'});

PIXELS = [];

$('#viewer_wrap #msg').hide().click(function() { $(this).fadeOut('def'); });

$('#play').click(function() { playPause(null) });
$('#first').click(function() { playPause(false); toFrame(1, true); });
$('#next').click(function() { playPause(false); toFrame(1, false); });
$('#previous').click(function() { playPause(false); toFrame(-1, false); });
$('#last').click(function() { playPause(false); toFrame(NB_FRAMES, true); });
$('#time_slider').slider({value: 1, min: 1, max: NB_FRAMES, step: 1, animate: true, stop: function(ev, ui) {
    toFrame(ui.value, true);
    if(PBS) { PBS = false; playPause(true); turnTimer(false); }
}, slide: function(ev, ui) {
    setTime(ui.value);
}, start: function(ev, ui) {
    if(PLAYING) { playPause(false); PBS = true; }
}});

var clone = $('#map').clone();
for(var y = 0; y < CONF.sizey; y ++) {
    var row = $('<div class="row"></div>');
    var pixrow = [];
    for(var x = 0; x < CONF.sizex; x ++) {
        var cell = $('<div class="cell"></div>');
        cell.attr('id', 'p_' + x + '_' + y);
        cell.css('background-position', COORDS['empty']);
        row.append(cell);
        pixrow.push(cell);
    }
    PIXELS.push(pixrow);
    clone.append(row);
}
$('#map').replaceWith(clone);

var href = window.location.hash.split('-'), gotoFrame = 1;
if(href.length == 2 && href[1] != '') {
    var gotoF = parseInt(href[1]);
    if(!isNaN(gotoF) && gotoF > 0 && gotoF <= NB_FRAMES)
        gotoFrame = gotoF;
}

toFrame(1, true);
if(gotoFrame != 1) toFrame(gotoFrame, true);
dlFrame();

});
