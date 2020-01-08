<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\MemcacheDriverInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use Zynga\Framework\Exception\V1\Exception;

/**
 * This is an in-memory cache for transient data. If you need to have data persist between requests, consider the Memcache driver
 */
class InMemory extends DriverBase implements MemcacheDriverInterface {
  private static Map<string, mixed> $data = Map {};
  private DriverConfigInterface $_config;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function directIncrement(string $key, int $incrementValue = 1): int {
    $value = self::$data->get($key);
    if (is_int($value)) {
      $value = $value + $incrementValue;
      self::$data->set($key, $value);
      return $value;
    }

    return 0;
  }

  public function directAdd(
    string $key,
    mixed $value,
    int $flags = 0,
    int $ttl = 0,
  ): bool {
    $getValue = self::$data->get($key);
    if($getValue !== null) {
      return false;
    }
  
    self::$data->set($key, $value);
    return true;
  }

  public function directDelete(string $key): bool {
    $data = self::$data->get($key);

    if ($data === null) {
      return false;
    }

    self::$data->remove($key);

    return true;
  }

  public function directSet(
    string $key,
    mixed $value,
    int $flags = 0,
    int $ttl = 0,
  ): bool {
    self::$data->set($key, $value);
    $valueSet = $this->directGet($key);
    return $valueSet === null ? false : true;
  }

  public function directGet(string $key): mixed {
    return self::$data->get($key);
  }

  public function connect(): bool {
    return true;
  }

  public function add(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      $value = $this->directGet($key);

      // mimic the atomic lock of memcache, if there's a value it's already set.
      if ($value !== null) {
        return false;
      }

      $this->directSet($key, $obj);

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function get(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): ?StorableObjectInterface {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);

      $storableObject = $this->directGet($key);

      // no data to work with.
      if ($storableObject === null ||
          !$storableObject instanceof StorableObjectInterface) {
        return null;
      }

      return $storableObject;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function set(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      $this->directSet($key, $obj);
      $storableObject = $this->directGet($key);

      return $storableObject === null ? false : true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function delete(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);

      $data = $this->directGet($key);

      if (!$data instanceof StorableObjectInterface) {
        return false;
      }

      self::$data->remove($key);

      return true;

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function clearInMemoryCache(): bool {
    self::$data = Map {};
    return true;
  }
}
