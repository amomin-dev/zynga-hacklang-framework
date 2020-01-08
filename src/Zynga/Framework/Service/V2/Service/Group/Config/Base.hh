<?hh // strict 

namespace Zynga\Framework\Service\V2\Service\Group\Config;

use Zynga\Framework\Service\V2\Service\Group\Config\Pattern;
use Zynga\Framework\Type\V1\StringBox;

abstract class Base {

  public StringBox $title;
  public StringBox $description;
  public Vector<Pattern> $patterns;

  public function __construct() {
    $this->title        = new StringBox();
    $this->description  = new StringBox();
    $this->patterns     = Vector {};
  }

}
