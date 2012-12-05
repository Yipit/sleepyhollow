(function($){$(function(){
    var i = 0;

    while (i <= 100) {
        $("body").append($('<div class="square"></div>'));
        i++;
    }

    $('.square').animate({ 'margin': '100px', 'border-width': '40px', 'width': '100px', 'height': '100px'}, 5000);
})})(jQuery);
