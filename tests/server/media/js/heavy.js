(function($){$(function(){
    var i = 0;

    var timer = setInterval(function(){
        $("body").prepend($('<div class="square" id="sq'+i+'"></div>'));
        $('#sq'+i).animate({ 'margin': '10px', 'border-width': '40px', 'width': '100px', 'height': '100px'}, 500);
        i++;
        if (i > 20) {
            clearInterval(timer);
        }
    }, 500);

})})(jQuery);
