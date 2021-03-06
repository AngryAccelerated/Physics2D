#include "testbed/window.h"


#include "include/dynamics/constraint/constraint.h"
#include "include/dynamics/constraint/contact.h"
#include "include/render/renderer.h"
#include <iostream>
namespace Physics2D
{
	Window::Window(QWidget* parent)
	{
		this->setParent(parent);
		this->setWindowTitle("Testbed");
		this->resize(1920, 1080);
		this->setMouseTracking(true);

		rectangle.set(1, 1);
		land.set(32, 0.2);
		polygon.append({{3, 0}, {2, 3}, {-2, 3}, {-3, 0}, {-2, -3}, {2, -3}, {3, 0}});
		polygon.scale(0.22);
		ellipse.set({-5, 4}, {5, -4});
		ellipse.scale(0.15);
		circle.setRadius(0.5);
		//circle.scale(7);
		edge.set({-18, 5}, {18, 0});
		capsule.set(1, 2);

		boxHorizontal.set({ -roomSize, 0 }, { roomSize, 0 });
		boxVertical.set({ 0, roomSize }, { 0, -roomSize });

		rectangle_ptr = std::make_shared<Rectangle>(rectangle);
		land_ptr = std::make_shared<Rectangle>(land);
		polygon_ptr = std::make_shared<Polygon>(polygon);
		ellipse_ptr = std::make_shared<Ellipse>(ellipse);
		circle_ptr = std::make_shared<Circle>(circle);
		edge_ptr = std::make_shared<Edge>(edge);
		capsule_ptr = std::make_shared<Capsule>(capsule);

		horizontalWall = std::make_shared<Edge>(boxHorizontal);
		verticalWall = std::make_shared<Edge>(boxVertical);
		



		m_world.setEnableGravity(true);
		m_world.setGravity({0, -0.98f});
		m_world.setLinearVelocityDamping(0.8f);
		m_world.setAirFrictionCoefficient(0.8f);
		m_world.setAngularVelocityDamping(0.8f);
		m_world.setPositionIteration(1);
		m_world.setVelocityIteration(1);
		
		//createStackBox(6, 1.1, 1.1);
		//createBoxRoom();
		//createBoxesAndGround(8);
		//testPendulum();
		//testCollision();
		testJoint();
		//testBroadphase();
		//testCCD();
		//testCapsule();
		
		camera.setViewport(Utils::Camera::Viewport((0, 0), (1920, 1080)));
		camera.setWorld(&m_world);
		camera.setDbvh(&dbvh);
		camera.setTree(&tree);
		
		camera.setAabbVisible(false);
		camera.setDbvhVisible(false);
		camera.setTreeVisible(false);
		camera.setAxisVisible(false);
		connect(&m_timer, &QTimer::timeout, this, &Window::process);

		
		m_timer.setInterval(15);
		m_timer.start();

		
	}

	Window::~Window()
	{

	}
	void Window::createBoxRoom()
	{


		ground = m_world.createBody();
		ground->setShape(horizontalWall);
		ground->position().set({ 0, (roomSize + 0.04) * 2 });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		//camera.setMeterToPixel(120);
		dbvh.insert(ground);

		ground = m_world.createBody();
		ground->setShape(verticalWall);
		ground->position().set({ (roomSize + 0.04), (roomSize + 0.04) });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		//camera.setMeterToPixel(120);
		dbvh.insert(ground);

		ground = m_world.createBody();
		ground->setShape(verticalWall);
		ground->position().set({ -(roomSize + 0.04), (roomSize + 0.04) });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		camera.setTargetBody(ground);
		//camera.setMeterToPixel(120);
		dbvh.insert(ground);

		ground = m_world.createBody();
		ground->setShape(horizontalWall);
		ground->position().set({ 0, -(0 + 0.04) });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		//camera.setMeterToPixel(120);
		dbvh.insert(ground);
	}
	void Window::testBroadphase()
	{
		for (int i = 0; i < 500; i++)
		{
			Body* body = m_world.createBody();
			body->position().set(-9.0 + QRandomGenerator::global()->bounded(18.0),
			                     -9.0 + QRandomGenerator::global()->bounded(18.0));
			body->setShape(rectangle_ptr);
			body->rotation() = -360 + QRandomGenerator::global()->bounded(720);
			body->setMass(400);
			body->setType(Body::BodyType::Static);
			
			dbvh.insert(body);
		}
	}

	void Window::testCCD()
	{
		rect = m_world.createBody();
		rect->setShape(rectangle_ptr);
		rect->position().set({ 8, -5.4 });
		rect->setMass(Constant::Max);
		rect->rotation() = 90;
		rect->setType(Body::BodyType::Dynamic);

		rect2 = m_world.createBody();
		rect2->setShape(rectangle_ptr);
		rect2->position().set({ -5.5, -6 });
		rect2->rotation() = 90;
		rect2->setMass(200);
		rect2->setType(Body::BodyType::Bullet);
		rect2->velocity() = { 1000, 0 };
		rect2->angularVelocity() = 0;

		rect3 = m_world.createBody();
		rect3->setShape(rectangle_ptr);
		rect3->position().set({ 8, -6.6 });
		rect3->rotation() = 180;
		rect3->setMass(200);
		rect3->setType(Body::BodyType::Dynamic);
		//rect3->angularVelocity() = 360;
		
		dbvh.insert(rect);
		dbvh.insert(rect2);
		dbvh.insert(rect3);
	}

	void Window::testCapsule()
	{
		rect = m_world.createBody();
		rect->setShape(capsule_ptr);
		rect->position().set({ 0, 0 });
		rect->rotation() = 0;
		rect->setMass(20);
		rect->setType(Body::BodyType::Dynamic);
		
		dbvh.insert(rect);
	}


	void Window::process()
	{
		if(isStop)
		{
			for (auto& body : m_world.bodyList())
				dbvh.update(body.get());
			repaint();
			return;
		}
		const real dt = 1.0 / 30.0;
		const real inv_dt = 30;

		
		m_world.stepVelocity(dt);

		for(int i = 0;i < 1;i++)
		{
			auto potentialList = dbvh.generatePairs();
			for (auto pair : potentialList)
			{
				auto result = Detector::detect(pair.first, pair.second);
				if (result.isColliding)
					contactMaintainer.add(result);
			}

			contactMaintainer.solve(dt);
		}
		

		m_world.stepPosition(dt);

		for (auto& body : m_world.bodyList())
			dbvh.update(body.get());
		

		
		repaint();
	}

	void Window::testJoint()
	{

		ground = m_world.createBody();
		ground->setShape(edge_ptr);
		ground->position().set({ 0, -10 });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);

		//tree.insert(ground);
		
		rect = m_world.createBody();
		rect->setShape(circle_ptr);
		rect->position().set({-0.5, -1});
		rect->rotation() = 0;
		rect->setMass(200);
		rect->setType(Body::BodyType::Dynamic);

		//rect2 = m_world.createBody();
		//rect2->setShape(rectangle_ptr);
		//rect2->position().set({-5, -5});
		//rect2->rotation() = 0;
		//rect2->setMass(200);
		//rect2->setFriction(0.8);
		//rect2->setType(Body::BodyType::Dynamic);
		

		//rotationPrim.bodyA = rect;
		//rotationPrim.bodyB = rect2;
		//rotationPrim.referenceRotation = Math::degreeToRadian(45);
		//RotationJoint* rj = m_world.createJoint(rotationPrim);

		distancePrim.bodyA = rect;
		distancePrim.localPointA.set({ 0, 0 });
		distancePrim.minDistance = 2;
		distancePrim.maxDistance = 4;
		distancePrim.targetPoint.set({ 1, 1 });
		m_world.createJoint(distancePrim);

		orientationPrim.bodyA = rect;
		orientationPrim.targetPoint.set({ 1, 1 });
		orientationPrim.referenceRotation = 0;
		m_world.createJoint(orientationPrim);

		//pointPrim.localPointA.set(-0.5, -0.5);
		//pointPrim.localPointB.set(0.5, 0.5);

		//pointPrim.localPointA.set(0, 0.5);
		//pointPrim.targetPoint.set(2.0, 2.0);
		//pointPrim.bodyA = rect;
		//m_world.createJoint(pointPrim);
		

		dbvh.insert(rect);
		//dbvh.insert(rect2);
		dbvh.insert(ground);
		camera.setTargetBody(rect);

		//rect->velocity() += {10, 0};

	}

	void Window::testCollision()
	{
		ground = m_world.createBody();
		ground->setShape(edge_ptr);
		ground->position().set({ 0, 0 });
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		ground->setFriction(0.7);

		dbvh.insert(ground);
		
		rect = m_world.createBody();
		rect->setShape(rectangle_ptr);
		rect->position().set({-5, 6});
		rect->rotation() = Math::degreeToRadian(5);
		rect->setMass(200);
		rect->setType(Body::BodyType::Dynamic);
		rect->setFriction(0.1);
		dbvh.insert(rect);

		//rect2 = m_world.createBody();
		//rect2->setShape(circle_ptr);
		//rect2->position().set({ 0, 2 });
		//rect2->rotation() = -20;
		//rect2->setMass(200);
		//rect2->setType(Body::BodyType::Static);
		//dbvh.insert(rect2);
		//rect->velocity().set(0.5, 0);
		//rect2->velocity().set(-0.5, 0);

		//rect->angularVelocity() = 15;
		//rect2->angularVelocity() = -15;

		camera.setTargetBody(rect);

		//rect->velocity().set(0, -4);
	}

	void Window::createSnakeBody()
	{
		for (size_t i = 0; i < 15; i++)
		{
			Body* rect = m_world.createBody();
			rect->setShape(rectangle_ptr);
			rect->position().set({-600 + static_cast<real>(i * 60), 250});
			rect->rotation() = 45;
			rect->setMass(2);
			rect->setType(Body::BodyType::Dynamic);
			if (i == 0)
				rect2 = rect;
		}
	}

	void Window::testDistanceJoint()
	{
	}

	void Window::testPendulum()
	{
		rect = m_world.createBody();
		rect->setShape(circle_ptr);
		rect->position().set({0, 4});
		rect->rotation() = 0;
		rect->setMass(Constant::Max);
		rect->setType(Body::BodyType::Kinematic);

		rect2 = m_world.createBody();
		rect2->setShape(rectangle_ptr);
		rect2->position().set(-12, -9);
		rect2->rotation() = 0;
		rect2->setMass(100);
		rect2->setType(Body::BodyType::Dynamic);
	}

	void Window::paintEvent(QPaintEvent*)
	{
		QPainter painter(this);
		camera.render(&painter);
		real dt = 1.0 / 60;

		//QPen pen3(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		//QPen pen2(Qt::green, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		//for(auto iter = contactMaintainer.m_contactTable.begin(); iter != contactMaintainer.m_contactTable.end(); ++iter)
		//{
		//	for(auto& elem:iter->second)
		//	{
		//		RendererQtImpl::renderPoint(&painter, &camera, elem.bodyA->toWorldPoint(elem.localA), pen2);
		//		RendererQtImpl::renderPoint(&painter, &camera, elem.bodyB->toWorldPoint(elem.localB), pen2);
		//	}
		//}

		//auto potentialList = dbvh.generatePairs();
		//for (auto pair : potentialList)
		//{
		//	auto result = Detector::detect(pair.first, pair.second);
		//	if (result.isColliding)
		//	{
		//		ShapePrimitive p1, p2;
		//		p1.rotation = result.bodyA->rotation();
		//		p1.shape = result.bodyA->shape();
		//		p1.transform = result.bodyA->position();


		//		p2.rotation = result.bodyB->rotation();
		//		p2.shape = result.bodyB->shape();
		//		p2.transform = result.bodyB->position();
		//		RendererQtImpl::renderShape(&painter, &camera, p1, pen3);
		//		RendererQtImpl::renderShape(&painter, &camera, p2, pen3);
		//		for(auto& elem: result.contactList)
		//		{
		//			RendererQtImpl::renderPoint(&painter, &camera, elem.pointA, pen2);
		//			RendererQtImpl::renderPoint(&painter, &camera, elem.pointB, pen2);
		//		}

		//	}
		//}

		//ShapePrimitive p1, p2;
		//p1.rotation = rect->rotation();
		//p1.shape = rect->shape();
		//p1.transform = rect->position();


		//p2.rotation = rect2->rotation();
		//p2.shape = rect2->shape();
		//p2.transform = rect2->position();
		//auto result = GJK::distance(p1, p2);
		//RendererQtImpl::renderLine(&painter, &camera, result.pointA, result.pointB, pen3);
		//
		//
		//auto result = Detector::detect(rect, rect2);
		//if(result.isColliding)
		//{

		//	//ShapePrimitive primitive;
		//	//primitive.shape = rect->shape();
		//	//primitive.rotation = rect->rotation();
		//	//primitive.transform = rect->position();
		//	//RendererQtImpl::renderShape(&painter, &camera, primitive, pen2);


		//	//primitive.shape = rect2->shape();
		//	//primitive.rotation = rect2->rotation();
		//	//primitive.transform = rect2->position();
		//	//RendererQtImpl::renderShape(&painter, &camera, primitive, pen2);

		//	RendererQtImpl::renderLine(&painter, &camera, result.contactList[0].pointA, result.contactList[0].pointB, pen2);
		//}
		
		//QPen pen(Qt::cyan, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		//auto result = CCD::query(dbvh.root(), rect2, dt);
		////fmt::print("toi exist:{}\n", result.has_value());
		//if (result.has_value())
		//{
		//	auto list = result.value();
		//	for(auto& pair: list)
		//	{
		//		fmt::print("toi:{}\n", pair.toi);
		//		Body::PhysicsAttribute origin = rect2->physicsAttribute();
		//		rect2->stepPosition(pair.toi);
		//		ShapePrimitive primitive;
		//		primitive.shape = rect2->shape();
		//		primitive.rotation = rect2->rotation();
		//		primitive.transform = rect2->position();
		//		RendererQtImpl::renderShape(&painter, &camera, primitive, pen2);
		//		rect2->setPhysicsAttribute(origin);
		//	}
		//}
	}

	void Window::resizeEvent(QResizeEvent* e)
	{

		camera.setViewport({ {0, 0}, {e->size().width() - camera.viewport().topLeft.x,
									   e->size().height() - camera.viewport().topLeft.y } });
		this->repaint();
	}

	void Window::mousePressEvent(QMouseEvent* e)
	{
		Vector2 pos(e->pos().x(), e->pos().y());
		mousePos = camera.screenToWorld(pos);
		if (e->button() == Qt::RightButton)
		{
			cameraTransform = true;
		}
		for(auto& body: m_world.bodyList())
		{
			Vector2 point = mousePos - body->position();
			point = Matrix2x2(-body->rotation()).multiply(point);
			if(body->shape()->contains(point) && selectedBody == nullptr)
			{
				selectedBody = body.get();
				break;
			}
		}
	}

	void Window::mouseReleaseEvent(QMouseEvent* e)
	{
		Vector2 pos(e->pos().x(), e->pos().y());
		mousePos = camera.screenToWorld(pos);

		clickPos.clear();
		cameraTransform = false;
		selectedBody = nullptr;
	}


	void Window::mouseMoveEvent(QMouseEvent* e)
	{
		//testHit(e->pos());

		Vector2 pos(e->pos().x(), e->pos().y());
		//originPoint.set(m_world.screenToWorld(pos));
		//mousePrim.mousePoint = mousePos;
		Vector2 tf = camera.screenToWorld(pos) - mousePos;
		if(cameraTransform)
		{
			tf *= camera.meterToPixel();
			camera.setTransform(camera.transform() + tf);
		}
		if(selectedBody != nullptr)
		{
			selectedBody->position() += tf;
		}
		mousePos = camera.screenToWorld(pos);
		//auto prim = mj->primitive();
		//prim.mousePoint = mousePos;
		//mj->set(prim);
		repaint();
	}

	void Window::mouseDoubleClickEvent(QMouseEvent* event)
	{
		Vector2 pos(event->x(), event->y());
		clickPos.set(camera.screenToWorld(pos));
        //fmt::print("select body at {}\n", clickPos);
			
	}

	void Window::keyPressEvent(QKeyEvent* event)
	{
		switch (event->key())
		{
		case Qt::Key_J:
			{
				camera.setJointVisible(!camera.jointVisible());
				break;
			}
		case Qt::Key_B:
			{
				camera.setBodyVisible(!camera.bodyVisible());
				break;
			}
		case Qt::Key_D:
			{
				camera.setDbvhVisible(!camera.dbvhVisible());
				break;
			}
		case Qt::Key_A:
			{
				camera.setAabbVisible(!camera.aabbVisible());
				break;
			}
		case Qt::Key_Space:
		{
			isStop = !isStop;
			break;
		}
		case Qt::Key_R:
			{
			rect->rotation() -= Math::degreeToRadian(5);
			break;
			}
		case Qt::Key_L:
		{
			if (camera.targetBody() != nullptr)
				camera.setTargetBody(nullptr);
			else
				camera.setTargetBody(rect);
			break;
		}
		default:
			break;
		}
		repaint();
	}

	void Window::keyReleaseEvent(QKeyEvent* event)
	{

		repaint();
	}

	void Window::wheelEvent(QWheelEvent* event)
	{
		//fmt::print("delta:{},{}\n", event->angleDelta().x(), event->angleDelta().y());
		if (event->angleDelta().y() > 0)
			camera.setMeterToPixel(camera.meterToPixel() + camera.meterToPixel() / 4.0);
		else
			camera.setMeterToPixel(camera.meterToPixel() - camera.meterToPixel() / 4.0);
		repaint();
	}
	

	void Window::createStackBox(const uint16_t& row = 10, const real& margin = 65, const real& spacing = 55)
	{
		for (real j = row; j > 0; j--)
		{
			for (real i = 0; i < 2 * (row - j) + 1; i++)
			{
				Body* body = m_world.createBody();
				body->position().set({
					(-spacing * (row - j) + i * spacing), j * margin + margin - 6
				});
				body->setShape(rectangle_ptr);
				body->rotation() = 0;
				body->setMass(400);
				body->setType(Body::BodyType::Dynamic);
				dbvh.insert(body);
			}
		}


		ground = m_world.createBody();
		ground->setShape(land_ptr);
		ground->position().set({0, -8});
		ground->setMass(Constant::Max);
		ground->setType(Body::BodyType::Static);
		dbvh.insert(ground);
	}

	void Window::createBoxesAndGround(const real& count)
	{
		for (real j = 0; j < count; j++)
		{
			for(real i = 0;i < count; i++)
			{
				Body* body = m_world.createBody();
				body->position().set({ i * 0.5 - 8, j * 0.5 + 1});
				body->setShape(rectangle_ptr);
				body->rotation() = 0;
				body->setMass(200);
				body->setType(Body::BodyType::Dynamic);
				body->setFriction(0.8);
				camera.setTargetBody(body);
				dbvh.insert(body);
			}
		}

		//ground = m_world.createBody();
		//ground->setShape(edge_ptr);
		//ground->position().set({0, -1.0});
		//ground->setMass(Constant::Max);
		//ground->setType(Body::BodyType::Static);
		//camera.setTargetBody(ground);
		//camera.setMeterToPixel(120);
		//dbvh.insert(ground);
	}
}
