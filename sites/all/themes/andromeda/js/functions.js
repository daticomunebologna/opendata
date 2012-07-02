jQuery(function () {
    if (jQuery('.content_follow') != undefined && jQuery('.content_follow').next(".content_ok").length == 0 ) {
         jQuery('.content_follow').parents('.center-wrapper').remove();
    }
        
    
});