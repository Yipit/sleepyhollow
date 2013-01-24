$(function(){
    $("li[data-path][data-description]").each(function(){
        var $self = $(this);
        var $link = $("<a>"+$self.data('description')+"</a>");
        $link.click(function(e){
            $link.text("Clicked at " + $self.data('description'));
            $link.attr("style", "color: #668800");
            return e.preventDefault();
        });
        $link.attr("href", "http://yipit.com/" + $self.data('path'));
        $self.empty().append($link);
    });
});