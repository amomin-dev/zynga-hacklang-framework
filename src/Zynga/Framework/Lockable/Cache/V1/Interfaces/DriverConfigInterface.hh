<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Interfaces;

use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;
use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverConfigInterface extends ConfigInterface {

  /**
   *
   * Return the appropriate cache object here, where data goes.
   *
   * @return CacheDriverInterface
   *
   */
  public function getCache(): CacheDriverInterface;

  /**
   *
   * Return the appropriate cache object here, where locks go.
   *
   * @return CacheDriverInterface
   *
   */
  public function getLockCache(): CacheDriverInterface;

  /**
   *
   * Most implementations use the default LockPayload, but you can overload it
   * if needed.
   *
   * @return LockPayLoadInterface
   *
   */
  public function getPayloadObject(): LockPayloadInterface;

  /**
   *
   * Defaults to the cache's TTL value, if you want it to be shorter
   * overload it here, however we do check that you are not setting a
   * value that is higher than the cache's supported TTL.
   *
   * @return int number of seconds
   *
   */
  public function getLockTTL(): int;

}
