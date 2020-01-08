<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\BadConnection;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Exception\V1\Exception;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

class Dev implements DriverConfigInterface {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    $hosts['172.1.1.1'] = 11211;
    return $hosts;
  }

  public function getDriver(): string {
    return 'Memcache';
  }

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    throw new InvalidObjectForKeyCreationException('not-valid-connection');
  }

  public function getTTL(): int {
    return 3600;
  }

  public function cacheAllowsKeyOverride(): bool {
    return false;
  }

  public function cacheAllowsNonExpiringKeys(): bool {
    return false;
  }

  public function cacheAllowsTTLOverride(): bool {
    return false;
  }

}
