/**
 * @file
 * jQuery to provide characters remaining counter.
 */

(function ($) {

Drupal.behaviors.buffer = {
  attach: function(context) {
    $(document).ready(function() {
      var countdown = {
        init: function() {
          countdown.remaining = countdown.max - $(countdown.obj).val().length;
          if ($("#edit-append-url").is(':checked')) {
            countdown.remaining = countdown.remaining - 23;
          }
          if (countdown.remaining < 0) {
            $(countdown.obj).parent().parent().find(".remaining").addClass("error");
          }
          else {
            $(countdown.obj).parent().parent().find(".remaining").removeClass("error");
          }     
          if (countdown.remaining > countdown.max) {
            $(countdown.obj).val($(countdown.obj).val().substring(0,countdown.max));
          }
          $(countdown.obj).parent().parent().find(".remaining").html(countdown.remaining + " " + Drupal.t("characters remaining."));
        },
        max: null,
        remaining: null,
        obj: null
      };

      $(".countdown").each(function() {
        $(this).focus(function() {
          var c = $(this).attr("class");
          countdown.max = parseInt(c.match(/limit_[0-9]{1,}_/)[0].match(/[0-9]{1,}/)[0]);
          countdown.obj = this;
          iCount = setInterval(countdown.init,1000);
        }).blur(function() {
          countdown.init();
          clearInterval(iCount);
        });
      });

      $("#edit-append-url").change(function() {
        if(this.checked) {
          countdown.remaining = countdown.remaining - 23;
        }
        else {
          countdown.remaining = countdown.remaining + 23;
        }
        $(countdown.obj).parent().parent().find(".remaining").html(countdown.remaining + " " + Drupal.t("characters remaining."));
      });
    });
  }
};

})(jQuery);
