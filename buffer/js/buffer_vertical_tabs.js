/**
 * @file
 * jQuery to provide summary information inside vertical tabs.
 */

(function ($) {

/**
 * Provide summary information for vertical tabs.
 */
Drupal.behaviors.buffer_settings = {
  attach: function (context) {

    // Provide summary during content type configuration.
    $('fieldset#edit-buffer', context).drupalSetSummary(function(context) {
      var vals = [];
      if ($('#edit-buffer--2', context).is(':checked')) {
        vals.push(Drupal.t('Enabled'));
      }
      else {
        vals.push(Drupal.t('Disabled'));
      }
      return vals.join('<br/>');
    });
  }
};

})(jQuery);
