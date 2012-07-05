<?php
/**
 * @file
 * Default theme implementation for a slideshow
 */
?>

<div class="andromeda-slideshow">
  <?php if ($images) : ?>
    <ul>
      <?php foreach ($images as $image) : ?>
        <li>
          <?php if ($image->image) : ?>
            <div class="image">
              <?php print $image->image; ?>
            </div>
          <?php endif; ?>
          <?php if ($image->title) : ?>
            <p class="image-title">
              <?php print $image->title; ?>
            </p>
          <?php endif; ?>
          <?php if ($image->caption) : ?>
            <p class="image-caption">
              <?php print $image->caption; ?>
            </p>
          <?php endif; ?>
        </li>
      <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>