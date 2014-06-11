<?php

/**
 * @file
 * Administrative page callbacks for the buffer module.
 */

/**
 * Builds the form for the buffer admin.
 */
function buffer_admin_form($form, &$form_state) {
  $form = array();

  $form['access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Token'),
    '#default_value' => variable_get('buffer_access_token', ''),
    '#required' => TRUE,
  );

  $profiles = buffer_get_profiles();
  if ($profiles !== FALSE) {
    $options = array();
    foreach ($profiles as $profile) {
      $options[$profile['id']] = $profile['formatted_service'] . ' (' . $profile['formatted_username'] . ')';
    }

    $form['profiles'] = array(
      '#title' => t('Profiles'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => variable_get('buffer_profile_ids', array()),
    );
  }
  else {
    drupal_set_message(t('There was a problem retrieving the profile list.'), 'error');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit function for buffer_admin_form.
 */
function buffer_admin_form_submit($form, &$form_state) {
  variable_set('buffer_access_token', trim($form_state['values']['access_token']));

  if (isset($form_state['values']['profiles'])) {
    variable_set('buffer_profile_ids', array_keys(array_filter($form_state['values']['profiles'])));
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Builds the form for view all pending updates.
 */
function buffer_scheduled_activity($form, &$form_state) {
  $form = array();

  $profiles = buffer_get_profiles();
  if ($profiles !== FALSE) {
    $buffer_profile_ids = variable_get('buffer_profile_ids', array());

    if (empty($buffer_profile_ids)) {
      drupal_set_message(t('You must select at least one profile on the <a href="@conf_link">configuration page</a>!', array('@conf_link' => '/admin/config/services/buffer')), 'error');

      return $form;
    }

    $form['activity'] = array(
      '#type' => 'fieldset',
      '#title' => t('SCHEDULED ACTIVITY'),
    );

    foreach ($profiles as $profile) {
      if (in_array($profile['id'], $buffer_profile_ids)) {
        $activity = buffer_get_pending_updates($profile['id']);

        if ($activity !== FALSE) {
          $form['activity'][$profile['id']] = _buffer_build_pending_updates_form($activity, $profile);
        }
      }
    }

    $form['activity']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete selected updates'),
      '#attributes' => array('onclick' => 'if(!confirm("' . t('Do you really want to delete the selected updates?') . '")){return false;}'),
    );
  }
  else {
    drupal_set_message(t('You must select at least one profile on the <a href="@conf_link">configuration page</a>!', array('@conf_link' => '/admin/config/services/buffer')), 'error');
  }

  return $form;
}

/**
 * Validation callback for buffer_scheduled_activity().
 */
function buffer_scheduled_activity_validate($form, &$form_state) {
  $updates = array();
  foreach ($activity as $p) {
    $updates = array_merge($updates, array_keys(array_filter($p)));
  }

  if (count($updates) == 0) {
    form_set_error('activity', t('Select at least one update to delete!'));
  }
}

/**
 * Submit function for buffer_scheduled_activity.
 */
function buffer_scheduled_activity_submit($form, &$form_state) {
  $access_token = 'access_token=' . variable_get('buffer_access_token', '');

  unset($form_state['values']['activity']['delete']);
  foreach ($form_state['values']['activity'] as $activity) {
    $updates = array_keys(array_filter($activity));

    foreach ($updates as $update) {
      $success = buffer_destroy_update($update);

      if ($success) {
        drupal_set_message(t('Your update has been deleted.'));
      }
      else {
        drupal_set_message(t('An error has been occurred during update deleting'), 'error');
      }
    }
  }
}
