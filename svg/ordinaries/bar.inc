<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

global $svg_chief, $options;
$subtype = $node->getAttribute('keyterm');
$height = 1000;
$palet_length = 1400;
$bend_length = 1600; // future - adjust for flag height???
if ($options['shape'] == 'flag') {
  $height = $options['flagHeight'];
  $palet_length = sprintf("%d", $height + 200);
}

// bar
// ( although a "bar sinister" is a bend sinister
if ($subtype == 'bar' && existModifierWithKeyterm($node,'sinister')) {
  $node->setAttribute('keyterm','bend');
  include 'svg/ordinaries/bend.inc';
} else {

  $bar_locs = array ( //  0    1     2       3          4          5             6               7                  8
    'all' => array (     'x', '1', '1,2', '1,2,3', '1,2,3,4', '1,2,3,4,5', '1,2,3,4,5,6', '1,2,3,4,5,6,7', '1,2,3,4,5,6,7,8' ),
    'barmod-up' => array (   'x', 'x', '1',   '1',     '1',       '1',         '1',           '1',             '1' ),
    'barmod-down' => array (    'x', 'x', '2',   '3',     '4',       '5',         '6',           '7',             '8' ),
    'barmod-in' => array (   'x', 'x', 'x',   '2',     '2,3',     '2,3,4',     '3,4',         '3,4,5',         '3,4,5,6' ),
    'barmod-out' => array (   'x', 'x', 'x',   '1,3',   '1,4',     '1,5',       '1,2,5,6',     '1,2,6,7',       '1,2,7,8' ),
  );
  $num = intval($node->getAttribute('number'));
  $bar_start = '-100';
  $bar_length = '1200';
  $palet_start = '-100';

  $linetype_size = 30;

  $main_linetype = getModifierByType($node, 'linetype');
  if ( $main_linetype == null ) $main_linetype = 'none';

  $mods = array_pad(array(), $num, $main_linetype );
  $couped = 'none';
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' ) {
      switch ( ($mod = $child->getAttribute('keyterm')) ) {
        case 'barmod-up':
        case 'barmod-down':
        case 'barmod-in':
        case 'barmod-out':
          $linetype = getModifierByType($child,'linetype');
          if ( $linetype == null ) $linetype = 'none';
          $locs = $bar_locs[$mod][$num];
          if ( $locs != 'x' ) foreach ( explode(',',$locs) as $pos ) $mods[$pos-1] = $linetype;
          $child->setAttribute('value','used');
          break;
        case 'couped':
          $bar_start = '100';
          $bar_length = '800';
          $palet_start = '100';
          $palet_length = sprintf("%d", $height - 200);;
          $child->setAttribute('value','used');
          break;
        case 'couped-base':
          $palet_start = '-100';
          $palet_length = sprintf("%d", $height - 200);;
          $child->setAttribute('value','used');
          break;
        case 'couped-chief':
          $palet_start = '100';
          $palet_length = sprintf("%d", $height - 200);
          $child->setAttribute('value','used');
          break;
        case 'couped-dexter':
          $bar_start = '100';
          $bar_length = '1000';
          $child->setAttribute('value','used');
          break;
        case 'couped-sinister':
          $bar_start = '-100';
          $bar_length = '1000';
          $child->setAttribute('value','used');
          break;
        case 'gemel':
          if ( $subtype == 'bar' or $subtype == 'palet' ) $subtype .= '-gemel';
          $child->setAttribute('value','used');
          break;
      }
    }
  }

  switch ($subtype) {
    case 'barrulet':
      $bar_width = 50;
      $linetype_size = 15;
      break;
    case 'endorse':
    case 'closet':
      $bar_width = 100;
      $linetype_size = 20;
      break;
    case 'bar-gemel':
    case 'palet-gemel':
    case 'couple-close':
      $linetype_size = 20;
      $bar_width = 200;
      break;
    case 'riband':
      $bar_width = 80;
      break;
    // case 'bar':
    // case 'palet':
    default:
      $bar_width = 200;
      if ($main_linetype == 'dancetty') $linetype_size = 50;
      break;
  }

  $vSpacing = $height/($num+1);
  $hSpacing = 1000/($num+1);

  if ( $num < 1 ) { $num = 1; }
  if ( $num > 8 ) { $num = 8; }
  $body = '';
  $spec = '';
  // Adjust bar size for height
  $H1 = $bar_width * $height / 1000;
  $H2 = $H1 / 2;
  for ( $i = 0; $i < $num; $i++ ) {
    switch ( $subtype ) {
      case 'barrulet':
      case 'closet':
      case 'bar':
        $Y1 = (($i+1) * $vSpacing) - $H2;
        $spec = "X{$bar_start}Y{$Y1}A{$bar_length}c{$H1}E{$bar_length}g{$H1}";
        break;
      case 'bar-gemel':
        $Y1 = (($i+1) * $vSpacing) - $H2;
        $Y2 = $Y1 + ($H1 * 0.75);
        $H4 = $H1 / 4;
        $spec = "X{$bar_start}Y{$Y1}A{$bar_length}c{$H4}E{$bar_length}g{$H4}   X{$bar_start}Y{$Y2}A{$bar_length}c{$H4}E{$bar_length}g{$H4}";
        break;
      case 'palet-gemel':
        $X1 = (($i+1) * $hSpacing) - $H2;
        $X2 = $X1 + ($H1 * 0.75);
        $H4 = $H1 / 4;
        $spec = "X{$X1}Y{$palet_start}a{$H4}C{$palet_length}e{$H4}G{$palet_length}    X{$X2}Y{$palet_start}a{$H4}C{$palet_length}e{$H4}G{$palet_length}";
        break;
      case 'couple-close':
        $W2 = ($H1/0.707) / 2;
        $X1 = (($i+1) * $hSpacing) - 500 - $W2;
        $Y1 = 500 - (($i+1) * $hSpacing);
        $H4 = $H1 / 4;
        $H3 = ($H1 / 4) * 3 * 0.707;
        if ( $svg_chief == 'CH' ) $Y1 += 300;
        $spec = "X{$X1}Y{$Y1}h{$H4}B{$bend_length}d{$H4}F{$bend_length}";
        $X2 = $X1 + $H3;
        $Y2 = $Y1 - $H3;
        $spec .= "   X{$X2}Y{$Y2}h{$H4}B{$bend_length}d{$H4}F{$bend_length}";
        break;
      case 'palet':
      case 'endorse':
        $X1 = (($i+1) * $hSpacing) - $H2;
        $spec = "X{$X1}Y{$palet_start}a{$H1}C{$palet_length}e{$H1}G{$palet_length}";
        break;
      case 'riband':
      case 'scarpe':
      case 'bendlet':
        $W2 = ($H1/0.707) / 2;
        $X1 = (($i+1) * $hSpacing) - 500 - $W2;
        $Y1 = 500 - (($i+1) * $hSpacing);
        if ( $svg_chief == 'CH' ) $Y1 += 300;
        $spec = "X{$X1}Y{$Y1}h{$H1}B{$bend_length}d{$H1}F{$bend_length}";
        break;
    }

    $body .= createBody( $spec, $mods[$i], $linetype_size );
    // we allow a single bar (only) to be cotticed
    if ( ($num == 1) && ($cot_node = getModifierNodeWithKeyterm($node,'cotticing', false)) != null ) {
      $cotiseSVG = '';
      $cot_linetype= getModifierByType($cot_node,'linetype');
			$cot_tinc = getTinctureNode($cot_node);
			// if ( !$cot_tinc )
			//  $cot_tinc = $tinc1;
      $numberCottices = $cot_node->getAttribute('value');
      for ( $i = 1; $i <= $numberCottices; $i++ )
			  $cotiseSVG .= createCotise( $spec, $i, $cot_linetype );
		  $cotiseSVG = apply_tincture ( $cot_tinc, $cotiseSVG );
      existModifierWithKeyterm($node,'cotticing',true); // mark as used
      $body .= $cotiseSVG;
		}
  }
  $ordinary['body'] = $body;
}
