<div class="nivo-slider theme-pascal">
  <?php if ($images) : ?>
      <?php foreach ($images as $image) : ?>
        <?php print $image->image_tag; ?>
      <?php endforeach; ?>
  <?php endif; ?>
</div>