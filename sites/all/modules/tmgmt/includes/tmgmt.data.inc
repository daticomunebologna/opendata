<?php

/**
 * @todo Make use of the TMGMTDataProvider in our Iterator.
 */
interface TMGMTDataProvider {

  /**
   * Extracts a structured data array from a TMGMTDataProvider
   */
  public function getSourceData();

}

/**
 * @todo Write an Iterator for iterating over the source data.
 */
class TMGMTDataObject implements Iterator {

  /**
   * (PHP 5 &gt;= 5.1.0)<br/>
   * Rewind the Iterator to the first element
   *
   * @link http://php.net/manual/en/iterator.rewind.php
   * @return void Any returned value is ignored.
   */
  public function rewind() {
    // TODO: Implement rewind() method.
  }

  /**
   * (PHP 5 &gt;= 5.1.0)<br/>
   * Return the current element
   *
   * @link http://php.net/manual/en/iterator.current.php
   * @return mixed Can return any type.
   */
  public function current() {
    // TODO: Implement current() method.
  }

  /**
   * (PHP 5 &gt;= 5.1.0)<br/>
   * Move forward to next element
   *
   * @link http://php.net/manual/en/iterator.next.php
   * @return void Any returned value is ignored.
   */
  public function next() {
    // TODO: Implement next() method.
  }

  /**
   * (PHP 5 &gt;= 5.1.0)<br/>
   * Return the key of the current element
   *
   * @link http://php.net/manual/en/iterator.key.php
   * @return scalar scalar on success, integer
   * 0 on failure.
   */
  public function key() {
    // TODO: Implement key() method.
  }

  /**
   * (PHP 5 &gt;= 5.1.0)<br/>
   * Checks if current position is valid
   *
   * @link http://php.net/manual/en/iterator.valid.php
   * @return boolean The return value will be casted to boolean and then evaluated.
   * Returns true on success or false on failure.
   */
  public function valid() {
    // TODO: Implement valid() method.
  }
}
