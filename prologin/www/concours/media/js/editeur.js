/** 
 * Core JS file for the Bombazéro editor 
 * Copyright (c) Zopieux 
 * 
 * Thanks to mickael for his work I'm based on 
 */ 

var TOOLS = ['brush', 'rect', 'erase']; 
var BLOCKS = ['empty', 'soft', 'wall', 'mesh', 'p1', 'p2', 'p3', 'p4']; 
var BLOCKS_U = {'p1':0, 'p2':0, 'p3':0, 'p4':0}; // 0 or whatever 
var grid = false; 
 
var $tabs;
var current_tool; 
var current_block; 
var isEditing = false; 
 
var event_pixel; 
var ctrlWasPressed = false; 
var shiftWasPressed = false; 
 
var xlock = 0; 
var ylock = 0; 
 
var x1, x2, y1, y2, xstep, ystep; 
 
var undoHistory = []; 
pixels = []; 
 
var UNIQ = {}; 
var latest_u; 

function loadFromSite() {
    var id = $('#selopen').val();
    if (id == null)
        alert('Aucune map disponible.'); 
    else { 
        $.get('/editeur/open/' + id + '/',
                function(data) { 
                    textToMap(data);
                    $tabs.tabs('select', 1);
                }
        );
    }
}

function loadFromRaw()
{
    textToMap($('#rawtt').val()); 
    $tabs.tabs('select', 1);
}

function loadFromCookie()
{ 
    var c = getCookies(); 
    
    if ('data' in c && c.data)
    {
        textToMap(c.data);
        $tabs.tabs('select', 1);
    }
    else 
        alert('Il n\'y a pas de données dans le cookie.') 
}

function saveToSite()
{ 
    var title = prompt('Indiquez un titre pour la map :').replace(/^\s+|\s+$/g, ''); 
    if (!title)
        return;
    
    $.post('save', {title: title, data : mapToText()},
            function(data)
            {
                alert("Votre map a bien été enregistrée");
            }
    ); 
}

function saveToCookie()
{
    setCookie('data', $('#rawtt').val());
    alert('Données sauvegardées.'); 
}

function getCookies() { 
    ret = {}; 
    cookies = document.cookie.split('; '); 
    for(var i = 0; i < cookies.length; i++) { 
        parts = cookies[i].split('='); 
        name = unescape(parts[0]); 
        value = unescape(parts[1]); 
        ret[name] = value; 
    } 
    return ret; 
} 
function setCookie(name, value) { 
    date = new Date(); 
    date.setTime(date.getTime() + 2*365*24*3600); 
    document.cookie = name + '=' + escape(value) + '; expires=' + date.toGMTString(); 
} 
 
function textToMap(data1) { 
    emptyMap(); 
    var data = data1.split('\n'); 
    for(var y = 0; y < CONF.sizey; y++) { 
        var row = data[y]; 
        if(!row) continue; 
        for(var x = 0; x < CONF.sizex; x++) { 
            d = row[x]; 
            if(!d) continue; 
            if(!d in GRAMMAR) d = ' '; 
            setPixelBlockAt(x, y, GRAMMAR[d]); 
        } 
    } 
} 
 
function mapToText() { 
    var out = ''; 
    for(var y = 0; y < CONF.sizey; y++) { 
        for(var x = 0; x < CONF.sizex; x++) { 
            out += GRAMMAR2[pixels[y][x].block]; 
        } 
        out += '\n'; 
    } 
    return out.substr(0, out.length - 1);
} 
 
function toggleGrid() { 
    grid = !grid; 
    if(grid) { 
        $('#map').addClass('bbgrid'); 
        //$('#map div.cell').addClass('grid'); 
    } else { 
        $('#map').removeClass('bbgrid'); 
        //$('#map div.cell').removeClass('grid'); 
    } 
} 
 
function checkCoherence() { 
    if(current_block in BLOCKS_U){ 
        $('#t_rect').hide(); 
        if(current_tool == 'rect') setTool('brush'); 
    } else { 
        $('#t_rect').show(); 
    } 
} 
 
function cleanMap() { 
    for(var y = 0; y < CONF.sizey; y++) { 
        for(var x = 0; x < CONF.sizex; x++) { 
            pixels[y][x].pixel.className = 'cell' + (grid ? ' grid' : ''); 
        } 
    } 
} 
 
/*function saveUndoData() { 
    var newData = pixels; 
    undoHistory.push(newData); 
} 
 
function undo() { 
    if(undoHistory.length > 0) 
        setData(undoHistory.pop()); 
}*/ 
 
function beginEditing(ev, pixel) { 
    //saveUndoData(); 
    event_pixel = pixel; 
    ctrlWasPressed = false; 
    shiftWasPressed = false; 
    isEditing = true; 
    pixelEvent(ev, pixel); 
} 
 
function stopEditing() { 
    if(isEditing) { 
        isEditing = false; 
        if(current_tool == 'rect') { 
            cleanMap(); 
            for(var x = x1; x != (x2 + xstep); x += xstep) { 
                for(var y = y1; y != (y2 + ystep); y += ystep) 
                    setPixelBlockAt(x, y, current_block); 
            } 
        } 
    } 
} 
 
function emptyMap() { 
    for(var y = 0; y < CONF.sizey; y++) { 
        for(var x = 0; x < CONF.sizex; x++) { 
            setPixelBlock(pixels[y][x].pixel, 'empty'); 
        } 
    } 
} 
 
function pixelEvent(ev, pixel) { 
    if(!shiftWasPressed && ev.shiftKey) 
        ylock = pixel.coords.y; 
 
    if(!ctrlWasPressed && ev.ctrlKey) 
        xlock = pixel.coords.x; 
 
    shiftWasPressed = ev.shiftKey; 
    ctrlWasPressed = ev.ctrlKey; 
 
    if(ev.shiftKey && pixel.coords.y != ylock) 
        pixel = getPixelAt(pixel.coords.x, ylock); 
 
    if(ev.ctrlKey && pixel.coords.x != xlock) 
        pixel = getPixelAt(xlock, pixel.coords.y); 
 
    if(current_tool == 'erase') 
        setPixelBlock(pixel, 'empty'); 
    else if(current_tool == 'rect') { 
        x1 = event_pixel.coords.x; 
        y1 = event_pixel.coords.y; 
        x2 = pixel.coords.x; 
        y2 = pixel.coords.y; 
 
        diffx = x2 - x1; 
        diffy = y2 - y1; 
 
        xstep = diffx >= 0 ? 1 : -1; 
        ystep = diffy >= 0 ? 1 : -1; 
 
        cleanMap(); 
        for(var x = x1; x != (x2 + xstep); x += xstep) { 
            for(var y = y1; y != (y2 + ystep); y += ystep) 
                pixels[y][x].pixel.className = 'cell rect' + (grid ? ' grid' : ''); 
        } 
    } else 
        setPixelBlock(pixel, current_block); 
} 
 
function showOpenTab(ui) { 
    $('#rawtt').val(''); 
    $('#selopen').load('/editeur/listmaps');
}

function showSaveTab() { 
    var data = mapToText();
    $('#rawtt').val(data);
    var logg = ''; 
    
    for (var d in BLOCKS_U) 
        if(data.indexOf(GRAMMAR2[d]) == -1) 
            logg += '<li class="err">Bloc ' + GRAMMAR2[d] + ' manquant</li>'; 
    
    if(data.indexOf(GRAMMAR2['mesh']) == -1) 
        logg += '<li class="warn">Pas de générateur de mèche</li>'; 
    
    logg += '<li>Génération terminée</li>'; 
    
    $('#log ul').empty().append(logg); 
    $('#rawtt').focus().select();
} 
 
function setBlock(new_block) { 
    current_block = new_block; 
    if(current_tool == 'erase') 
        setTool('brush'); 
    checkCoherence(); 
    for(var b in BLOCKS) { 
        var block = BLOCKS[b]; 
        btnObj = document.getElementById('b_' + block); 
        if(!btnObj) continue; 
        btnObj.className = (block == new_block ? 'tbitem selected' : 'tbitem'); 
    } 
} 
 
function setTool(new_tool) { 
    current_tool = new_tool; 
    checkCoherence(); 
    for(var i in TOOLS) { 
        var tool = TOOLS[i]; 
        btnObj = document.getElementById('t_' + tool); 
        btnObj.className = (tool == new_tool ? 'tbitem selected' : 'tbitem'); 
    } 
} 
 
function getEvent(ev) { 
    if(!ev && window.event) 
        ev = window.event; 
    return ev; 
} 
 
function getEventTarget(ev) { 
    if(ev.target) 
        return ev.target; 
    else if(ev.srcElement) 
        return ev.srcElement; 
} 
 
function getPixelAt(x, y) { 
    return pixels[y][x].pixel; 
} 
 
function getPixelBlockAt(x, y) { 
    return pixels[y][x].block; 
} 
 
function setPixelBlock(pixel, new_block) { 
    setPixelBlockAt(pixel.coords.x, pixel.coords.y, new_block); 
} 
 
function setPixelBlockAt(x, y, new_block) { 
    if(getPixelBlockAt(x, y) != new_block) { 
        var pixel = pixels[y][x]; 
        if(pixel.block in BLOCKS_U) 
            UNIQ[pixel.block] = null; 
        pixel.block = new_block; 
        pixel.pixel.style.backgroundPosition = COORDS[new_block]; 
        if(new_block in BLOCKS_U) { 
            latest_u = UNIQ[new_block]; 
            if(latest_u) 
                setPixelBlock(latest_u, 'empty'); 
            UNIQ[new_block] = pixel.pixel; 
        } 
    } 
} 
 
$(function() {
    $('#edit_tabs').tabs({
        'selected': 1,
        'show' :
            function(event, ui) {
                if (ui.index == 1)
                    return;

                // We move the textarea from one tab to another
                cloned = $('#rawtt').clone();
                $('#rawtt').remove();
                $(ui.panel).children('.leftcol').eq(0).prepend(cloned);

                if (ui.index == 0)
                    showOpenTab();
                else
                    showSaveTab();
            }
    });
 
    var image = document.getElementById('map'); 
    document.onmouseup = stopEditing;
    
    $tabs = $('#edit_tabs').tabs();

    $('#t_brush').click(function() { setTool('brush'); }); 
    $('#t_rect').click(function() { setTool('rect'); }); 
    $('#t_erase').click(function() { setTool('erase'); }); 
    $('#b_wall').click(function() { setBlock('wall'); }); 
    $('#b_soft').click(function() { setBlock('soft'); }); 
    $('#b_mesh').click(function() { setBlock('mesh'); }); 
    $('#b_p1').click(function() { setBlock('p1'); }); 
    $('#b_p2').click(function() { setBlock('p2'); }); 
    $('#b_p3').click(function() { setBlock('p3'); }); 
    $('#b_p4').click(function() { setBlock('p4'); }); 
    $('#a_empty').click(emptyMap); 
    $('#a_grid').click(toggleGrid); 
    $('#map').css({width: CONF.sizex*CONF.sprite_size, height: CONF.sizey*CONF.sprite_size}); 
    $('#rawtt').attr({rows: CONF.sizey, cols: CONF.sizex});
    
    $('#bt_loadfromsite').click(loadFromSite);
    $('#bt_loadfromcookie').click(loadFromCookie);
    $('#bt_loadfromraw').click(loadFromRaw);

    $('#bt_savetosite').click(saveToSite);
    $('#bt_savetocookie').click(saveToCookie);
 
    setTool('brush'); 
    setBlock('wall'); 
 
    for(var y = 0; y < CONF.sizey; y++){ 
        var row = document.createElement('div'); 
        row.className = 'row'; 
 
        var pixrow = []; 
 
        for (var x = 0; x < CONF.sizex; x++){ 
            var cell = document.createElement('div'); 
            cell.className = 'cell'; 
            cell.id = 'pixel_' + x + '_' + y; 
            cell.coords =  {'x' : x, 'y' : y}; 
 
            cell.onmousedown = function(ev) { 
                ev = getEvent(ev); 
                pixel = getEventTarget(ev); 
                beginEditing(ev, pixel); 
                return false; 
            } 
 
            cell.onmouseover = function(ev) { 
                ev = getEvent(ev); 
                pixel = getEventTarget(ev); 
 
                if (isEditing) 
                    pixelEvent(ev, pixel); 
            } 
 
            cell.style.backgroundPosition = COORDS['empty']; 
            //cell.innerHTML = '&nbsp;'; // useless 
 
            pixrow.push({ 
                'block' : 'empty', 
                'pixel' : cell 
            }); 
            row.appendChild(cell); 
        } 
        image.appendChild(row); 
        pixels.push(pixrow); 
    } 
     
/*    $("#dialog").dialog({ 
    bgiframe: true, 
    autoOpen: false, 
    width: 520, 
    modal: true, 
    draggable: false, 
    position: ['center', 20] 
    }); */
 
}); 
