<?php

/**
 * @file
 * Theme and preprocess functions for Andromeda Slideshow
 */

function theme_andromeda_slideshow_manage_form_table($variables) {
  $slideshow = $variables['form']['#slideshow'];
  $images = $variables['form'];
  $output = '';
  
  $table_id = 'slideshows-manage-' . $slideshow->sid;
  $table_classes = array('slideshow-manage-table');
  
  $rows = array();
  foreach ($images as $siid => $image) {
    
    if (is_int($siid)) {
      $img = $image['#image'];
      
      $thumb = theme('image_style', array('style_name' => 'andromeda_slideshow_thumb', 'path' => $img->uri));
      
      $links = array(
        'edit' => array(
          'title' => t('Edit'),
          'href' => 'admin/structure/slideshows/manage/' . $slideshow->sid . '/edit/' . $img->siid,
        ),
        'delete' => array(
          'title' => t('Delete'),
          'href' => 'admin/structure/slideshows/manage/' . $slideshow->sid . '/delete/' . $img->siid,
        ),
      );
      
      $operations = theme('links__ctools_dropbutton', array('links' => $links));
      
      $row = array();
      $row[] = array('data' => $thumb, 'class' => array('col-thumb'));
      $row[] = array('data' => $img->title, 'class' => array('col-title'));
      $row[] = array('data' => drupal_render($image['position']));
      $row[] = array('data' => $operations, 'class' => array('col-operations'));
      
      $rows[] = array(
        'data'  => $row,
        'class' => array('draggable'),
      );
    }
  }
  
  if (empty($rows)) {
    $add_link = l(t('Add an image'), 'admin/structure/slideshows/manage/' . $slideshow->sid . '/add');
    $rows[] = array(array('data' => t('Slideshow empty. !add.', array('!add' => $add_link)), 'colspan' => 4));
  }

  $header = array(t('Preview'), t('Title'), t('Weight'), t('Operations'));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => $table_classes)));
  
  drupal_add_tabledrag('slideshows-manage-' . $slideshow->sid, 'order', 'sibling', 'col-weight');
  
  return $output;
}