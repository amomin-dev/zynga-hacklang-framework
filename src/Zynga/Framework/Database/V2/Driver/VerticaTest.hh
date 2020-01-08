<?hh //strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Database\V2\Config\Test\Vertica\Dev as MockConfig;

use Zynga\Framework\Database\V2\Driver\Vertica as BaseDriver;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Database\V2\Exceptions\MockQueriesRequired;
use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class VerticaTest extends TestCase {

  //public function hideLogs(): bool { return false; }

  public function testConfig(): void {
    $config = new MockConfig();
    $this->assertEquals(false, $config->isDatabaseReadOnly());
    $this->assertEquals(
      'host=localhost user=zframework password=i-am-a-walrus port=5432 dbname=phpunit connect_timeout=5',
      $config->getConnectionString(),
    );
  }

  public function testGetQuoter(): void {
    $driver = new BaseDriver(new MockConfig());
    $this->assertTrue($driver->getQuoter() instanceof QuoteInterface);
  }

  public function testGetTransaction(): void {
    $driver = new BaseDriver(new MockConfig());
    $this->assertTrue(
      $driver->getTransaction() instanceof TransactionInterface,
    );
  }

  public function testSetIsConnected(): void {
    $driver = new BaseDriver(new MockConfig());
    $this->assertTrue($driver->setIsConnected(true));
  }

  public function testGetIsConnected(): void {
    $driver = new BaseDriver(new MockConfig());
    $driver->setIsConnected(true);
    $this->assertTrue($driver->getIsConnected());
  }

  public function testConnectWithStringWhileConnected(): void {
    $driver = new BaseDriver(new MockConfig());
    $driver->setIsConnected(true);
    $this->assertTrue($driver->connectWithString(''));
  }

  public function testConnectWithStringWhileDisconnected(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $driver->setIsConnected(false);
    $this->assertTrue(
      $driver->connectWithString($config->getConnectionString()),
    );
  }

  public function testConnect(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $this->assertTrue($driver->connect());
  }

  public function testConnect_BrokenData(): void {
    $config = new MockConfig();
    $config->setCurrentServer('not-a-real-server-'.mt_rand(1, 10000));
    $config->setPassword('bad-password-'.mt_rand(1, 100000));
    $driver = new BaseDriver($config);
    $this->assertFalse($driver->connect());
    $this->assertTrue($driver->hadError());
  }

  public function testDisconnect(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $this->assertTrue($driver->connect());
    $this->assertTrue($driver->disconnect());
  }

  public function testOnDriverConnectionChangeDoesNotAlterConnected(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $driver->setIsConnected(true);
    $this->assertTrue($driver->getIsConnected());
    $driver->onDriverConnectionChange(true, false);
    $this->assertTrue($driver->getIsConnected());
  }

  public function testDefaultHadError(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $this->assertFalse($driver->hadError());
  }

  public function testDefaultGetLastError(): void {
    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $this->assertEquals(0, strlen($driver->getLastError()));
  }

  public function test_query_brokenTable(): void {

    $config = new MockConfig();
    $driver = new BaseDriver($config);

    // Cannot use expectedException here as it causes the rest of the asserts
    // to not be fully execeuted.
    try {
      $driver->query('SELECT FROM invalidschema.invalidtablename');
    } catch (QueryFailedException $e) {
    }

    $this->assertTrue($driver->hadError());
    $this->assertNotEmpty($driver->getLastError());

  }

  public function test_query_NotRespectingMock(): void {

    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $driver->enableRequireMockQueries();
    $this->assertTrue($driver->getRequiresMockQueries());
    $this->expectException(MockQueriesRequired::class);
    $driver->query('SELECT FROM pht.hands');

  }

  public function test_query_ConnectedButNot(): void {

    $config = new MockConfig();
    $driver = new BaseDriver($config);
    $driver->setIsConnected(true);
    $this->expectException(ConnectionGoneAwayException::class);
    $driver->query('SELECT FROM pht.hands');

  }

}
