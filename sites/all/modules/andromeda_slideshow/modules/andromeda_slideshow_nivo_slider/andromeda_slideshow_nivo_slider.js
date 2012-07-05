(function($) {
  Drupal.behaviors.andromeda_slideshow_nivo_slider = {
    attach: function(context) {
      $('.nivo-slider', context).nivoSlider({
        directionNav: false
      });
    }
  }
})(jQuery);