<?hh //strict

namespace Zynga\Framework\Type\V2;

use \DateTime;
use \Exception;
use Zynga\Framework\Type\V1\Base as BaseBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromBoolException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

/**
 * Given a string, this will ensure that it is a valid DateTime format.
 * This box ensures we export the box in the format('Y-m-d H:i:s')
 */
class DateTimeBox extends BaseBox {

  protected DateTime $value;

  <<__Override>>
  public function __construct() {
    parent::__construct();
    $this->value = new DateTime();
  }

  public function __toString(): string {
    $stringValue = $this->value->format('Y-m-d H:i:s');
    if ($stringValue === false || $this->isDefaultValue()[0]) {
      $stringValue = "Unknown DateTime";
    }

    return $stringValue;
  }

  <<__Override>>
  public function reset(): bool {
    $this->value = new DateTime();
    $this->setIsDefaultValue(true);
    return true;
  }

  <<__Override>>
  public function get(): string {
    return $this->__toString();
  }

  public function isStringValid(string $value): bool {
    $dateTime = new self();
    try {
      $dateTime->set($value);
      return true;
    } catch (FailedToImportFromStringException $e) {
      return false;
    }
  }

  <<__Override>>
  public function set(mixed $value): bool {
    if ($value instanceof DateTime) {
      $this->value = $value;
      $this->setIsDefaultValue(false);
      return true;
    } else {
      return parent::set($value);
    }
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
    throw new FailedToImportFromBoolException();
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    throw new FailedToImportFromFloatException();
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    throw new FailedToImportFromIntegerException();
  }

  <<__Override>>
  protected function importFromString(string $value): bool {
    try {
      $dateTime = new DateTime($value);
      if ($dateTime !== false) {
        $this->value = $dateTime;
        return true;
      }
    } catch (Exception $exception) {
    }

    throw new FailedToImportFromStringException(
      "Unable to import string. value=$value",
    );
  }

}
