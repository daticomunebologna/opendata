<?php

/**
 * @file
 * Helper functions for Andromeda Slideshow
 */

/**
 * Returns a list of image styles available
 */
function andromeda_slideshow_get_image_styles() {
  //prepare a #options ready list of image styles
  $image_styles = array();
  foreach (image_styles() as $key => $style) {
    $image_styles[$key] = $style['name'];
  }
  
  //remove the default image style andromeda_slideshow_thumbnail from the list
  unset($image_styles['andromeda_slideshow_thumb']);
  
  return $image_styles;
}

/**
 * Enables a slideshow
 *
 * @param $sid
 *  The slideshow sid
 *
 * @param $redirect
 *  path to redirect to after a slideshow is enabled
 */
function andromeda_slideshow_enable_slideshow($sid, $redirect = FALSE) {
  $slideshow = andromeda_slideshow_load($sid);
  $slideshow->settings['status'] = 1;
  
  //save the slideshow
  $slideshow = andromeda_slideshow_save_slideshow($slideshow);
  
  //redirect if set
  if ($redirect) {
    drupal_goto($redirect);
  }
  
  return $slideshow;
}

/**
 * Disables a slideshow
 *
 * @param $sid
 *  The slideshow sid
 *  
 * @param $redirect
 *  path to redirect to after a slideshow is disabled
 */
function andromeda_slideshow_disable_slideshow($sid, $redirect = FALSE) {
  $slideshow = andromeda_slideshow_load($sid);
  $slideshow->settings['status'] = 0;
  
  //save the slideshow
  $slideshow = andromeda_slideshow_save_slideshow($slideshow);
  
  //redirect if set
  if ($redirect) {
    drupal_goto($redirect);
  }
  
  return $slideshow;
}

/**
 * Returns a list of enabled slideshows
 */
function andromeda_slideshow_get_enabled_slideshows() {
  $slideshows = andromeda_slideshow_load_slideshows();
  
  //remove disabled slideshows
  foreach ($slideshows as $sid => $slideshow) {
    if (isset($slideshow->settings['status']) && $slideshow->settings['status'] == 0) {
      unset($slideshows[$sid]);
    }
  }
  
  return $slideshows;
}

/**
 * Returns a list of disabled slideshows
 */
function andromeda_slideshow_get_disabled_slideshows() {
  $slideshows = andromeda_slideshow_load_slideshows();
  
  //remove enabled slideshows
  foreach ($slideshows as $sid => $slideshow) {
    if ($slideshow->settings['status'] == 1) {
      unset($slideshows[$sid]);
    }
  }
  
  return $slideshows;
}